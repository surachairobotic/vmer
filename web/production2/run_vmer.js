'use strict';

const Promise = require('bluebird');
const express = require('express');
var http = require('http');
var app = express();
const bodyParser = require('body-parser');
var multer  = require('multer');
var multer_upload = null;
var PATH = require('path');
var archiver = require('archiver');
var extract = require('extract-zip');
var my = require('./my');
var fs = require('fs');
var dgram = require('dgram').createSocket('udp4');
const child_process = require('child_process');

var server = http.createServer(function (req, res) {
  app(req,res);
});

function init(){
  try{
    my.init();
    multer_upload = multer({ dest: my.config.tmp_dir });
    init_web();
    init_udp();
    server.on('error', function (e) {
      my.log('warn', e);
    });
    server.on('listening', function (e) {
      for(var i = 0;i < process.argv.length; i++){
        if( process.argv[i]=='show_start_popup' ){
          my.alert('The program started properly','Info');
          break;
        }
      }
    });
    server.listen( my.config.port, '0.0.0.0');
  }
  catch(e){
    try{
      my.log('warn', e);
      if( typeof(e)=='string' ){
        e+= '\n'+'See more detailes in : '+PATH.resolve(my.get_logfile_name());
      }
    }
    catch(e){}
    my.alert(e);
  }
}

// for discovering devices
function init_udp(){
  dgram.on('listening', function () {
    var address = dgram.address();
    my.log('info','UDP Client listening on ' + address.address + ":" + address.port);
  });

  dgram.on('message', function (msg, rinfo) {
    my.log('info','Message from: ' + rinfo.address + ':' + rinfo.port +' - ' + msg);
    if(msg=='discover_devices'){
      return get_device_name()
      .then((device_name)=>{
        var res = Buffer.from(JSON.stringify({
          result: 'success',
          device_id: my.config.device_id,
          device_name: device_name,
        }));
        dgram.send( res, 0, res.length, rinfo.port, rinfo.address, ()=>{
          my.log('info', 'discover_devices sent back');
        });
      });
    }
  });

  dgram.on('error', function (e) {
    if( e.message.indexOf('EADDRINUSE') ){
      my.alert('Port '+my.config.port_udp+' already in use.\nThe program may already be running.');
    }
  });

  try{
    dgram.bind(my.config.port_udp);
  }
  catch(e){
    my.alert(e, true);
    process.exit();
  }
}

function init_web(){
  app.set('port', my.config.port);
  app.use(bodyParser.urlencoded({
      extended: true
  }));
  app.use(bodyParser.json());

  app.get('/api/:api', function(req,res){
    var data;
    if( typeof(req.query.data)=='string' ){
      try{
        data = JSON.parse(req.query.data);
      }
      catch(err){
        data = null;
      }
    }
    else{
      data = req.query.data;
    }
    return process_api( req, res, req.params.api, data )
    .catch((e)=>{
      api_error(req, res, e);
    });
  });

  app.post('/api/:api', multer_upload.single('file'), function (req, res, next) {
    my.log('info','post : '+req.params.api);
    var arr = req.file.originalname.split('.');
    if( arr[arr.length-1]!='zip' ){
      api_error(req, res, 'post : Invalid file ext:'+req.file.originalname);
      return;
    }
    return extract_zip( req.file.path )
    .then((ret)=>{
      if( req.params.api=='upload_routes' ){
        //return my.del(my.config.route_dir, false)
        //.then(()=>{
        return check_exist(my.config.route_dir)
        .then((b_exist)=>{
          if( b_exist )
            return Promise.resolve();
          else
            return fs.promises.mkdir(my.config.route_dir);
        }).then(()=>{
          return my.copy_all( ret.dir, my.config.route_dir );
        }).then(()=>{
          return get_route_list();
        }).then((routes)=>{
          res.send({
            result: 'success',
            routes: routes,
          });
          my.log('info','upload_route completed');
        }).finally(()=>{
          return my.del(ret.dir, false);
        });
      }
      else{
        return Promise.reject('Invalid post api name : '+req.params.api);
      }
    })
    .catch((e)=>{
      api_error(req, res, e);
    })
    .finally(()=>{
      fs.promises.unlink(req.file.path);
    });
  });
}


// send error message back to client if error with API
function api_error(req, res, err){
  var msg = (err && typeof(err)=='string') ? err : 'Unknown error';
  res.send({
    result: 'fail',
    msg: msg
  });
  if( err )
    console.log('api error : ',err);
};


function process_api( req, res, api_name, data ){
  if( api_name=='get_device_info' ){
    return get_device_name()
    .then((device_name)=>{
      res.send({
        result: 'success',
        device_id: my.config.device_id,
        device_name: device_name,
      });
    });
  }
  else if( api_name=='get_route_list' ){
    return get_route_list()
    .then((routes)=>{
      res.send({
        result: 'success',
        routes: routes,
      });
    });
  }
  else if( api_name=='remove_routes' ){
    my.log('info','remove_route : ',data.route_names);
    if( !data.route_names ){
      my.log('warn','remove_route : no route name');
      return Promise.reject('No route name');
    }

    // remove all routes
    if( data.route_names=='*' ){
      return my.del(my.config.route_dir, false)
      .then(()=>{
        return check_exist(my.config.route_dir);
      }).then((ret)=>{
        if( ret )
          return Promise.resolve();
        else
          return fs.promises.mkdir(my.config.route_dir);
      }).then(()=>{
        return get_route_list();
      }).then((routes)=>{
        res.send({
          result: 'success',
          routes: routes,
        });
      });
    }
    else if( !Array.isArray(data.route_names) ){
      my.log('warn','remove_route : route_names is not an array');
      return Promise.reject('Route names are not in an array');
    }

    var route_dir = PATH.resolve( my.config.route_dir );
    return Promise.all( data.route_names.map( route_name =>{
      if( typeof(route_name)!='string' ){
        my.log('warn','remove_route : route name is not a string : ',route_name);
        return Promise.reject('Route name is not a string : '+JSON.stringify(route_name));
      }
      let path = PATH.resolve(PATH.join( route_dir, route_name ));
      if( route_name.indexOf('..')>=0
          || path.indexOf(route_dir)<0
          || path==route_dir ){
        my.log('warn','remove_route : invalid route name : ',route_name);
        return Promise.reject('Invalid route name : '+route_name);
      }
      return check_exist(path)
      .then((ret)=>{
        if( !ret ){
          my.log('warn','remove_route : route not exist');
          return Promise.reject('The route does not exist : '+route_name);
        }
      });
    })).then(()=>{
      return Promise.all( data.route_names.map( route_name =>{
        return my.del( PATH.resolve(PATH.join( route_dir, route_name )) );
      }));
    }).then(()=>{
      return get_route_list();
    }).then((routes)=>{
      res.send({
        result: 'success',
        routes: routes,
      });
    });
    /*
    var path = PATH.resolve(PATH.join( route_dir, data.route_name ));
//    console.log(route_dir,path);
    if( data.route_name.indexOf('..')>=0
        || path.indexOf(route_dir)<0
        || path==route_dir ){
      my.log('warn','remove_route : invalid route name');
      return Promise.reject('Invalid route name');
    }
    return check_exist(path)
    .then((ret)=>{
      if( !ret ){
        my.log('warn','remove_route : route not exist');
        return Promise.reject('The route does not exist');
      }
      return my.del( path );
    }).then(()=>{
      res.send({
        result: 'success'
      });
    });*/
  }
  else if( api_name=='get_data_list' ){
    return fs.promises.readdir( my.config.result_dir )
    .then((files)=>{
      res.send({
        result: 'success',
        data_names: files,
      });
    });
  }
  else if( api_name=='download_data' ){
    var tmp_file = null, files = [];
    var fn_gen_tmp_file_path = function(cnt=0){
      var path = PATH.join(my.config.tmp_dir, my.create_random_string(10)+'.zip');
      return check_exist(path)
      .then((ret)=>{
        if( ret ){
          if( cnt > 10 ){
            my.log('warn', 'fn_gen_tmp_file_path : too many attemp : '+path);
            return Promise.reject();
          }
          return fn_gen_tmp_file_path(cnt+1);
        }
        else
          return Promise.resolve(path);
      });
    };

    return fn_gen_tmp_file_path()
    .then((path)=>{
      tmp_file = path;
      return fs.promises.readdir( my.config.result_dir );
    }).then((_files)=>{
      //files = _files;

      if( data.data_names=='*' ){
        files = _files;
      }
      else if( !Array.isArray(data.data_names) ){
        my.log('warn', 'download_data : data_names is not an array : '+JSON.stringify(data.data_names));
        return Promise.reject('Data names are not in an array');
      }
      else{
        files = data.data_names;
        for(var i=0;i<files.length;i++){
          if( !_files.includes(files[i]) ){
            my.log('warn', 'download_data : file not found : '+files[i]);
            return Promise.reject('Data file not found : '+files[i]);
          }
        }
      }

      if( files.length==0 ){
        my.log('info', 'download_data : no result file');
        res.status(204).send('No result file');
        return Promise.resolve();
      }
      return new Promise((resolve,reject)=>{
        var output = fs.createWriteStream(tmp_file);
        var archive = archiver('zip');

        archive.on('error', function(err){
          reject(err);
        });
        archive.pipe(output);
        files.forEach((f)=>{
          console.log(PATH.join(my.config.result_dir, f));
          //archive.file(PATH.join(my.config.result_dir, f), {name: f});
          archive.directory(PATH.join(my.config.result_dir, f), f);
        });

        output.on("close", function(){
          resolve();
        });
        archive.finalize();
      }).then(()=>{
        return new Promise((resolve,reject)=>{
          res.download(tmp_file, 'results.zip', (err)=>{
            if (err) {
              my.log('warn', err);
              reject();
              return;
            }
            my.log('info', 'download_data : file sent');
            my.del( tmp_file, false );
            //my.del( files.map( f => PATH.join(my.config.result_dir, f) ), false );
            resolve();
          });
        });
      });
    })
    .catch((e)=>{
      try{
        my.log('warn', e);
        if( tmp_file )
          my.del(tmp_file,false);
        res.status(500).send({result: 'fail', msg: typeof(e)=='string' ? e : 'Unexpected error'});
      }
      catch(e){
        my.log('warn', e);
      }
    });
  }
  else{
    return Promise.reject('invalid get api : '+api_name);
  }
}


function extract_zip(src, dst, cnt=0){
  var dir = PATH.join(my.config.tmp_dir, my.create_random_string(12));
  return check_exist(dir)
  .then((ret)=>{
    if( ret ){
      if( cnt < 10 )
        return resolve( extract_zip( src, dst, cnt+1 ) );
      else{
        my.log('error', 'extract_zip : too many attemp : src = '+src+', dst = '+dst);
        return reject();
      }
    }
    return extract( src, {dir: dir} );
  }).then(()=>{
    return fs.promises.readdir(dir);
  }).then((files)=>{
    return Promise.resolve({'dir': dir, 'files': files.map(f=>PATH.join(dir,f))});
  })
  .catch((e)=>{
    my.del(dir, false);
    return Promise.reject(e);
  });
}

function get_device_name(){
  //wmic.exe useraccount where "localaccount=true" get fullname
  return new Promise((resolve,reject)=>{
    var proc = child_process.execFile('wmic.exe', ['useraccount', 'where','localaccount=true','get','fullname'], {windowsHide : true}, (err, stdout, stderr) => {
      if (err) {
        my.log('error', 'get_username : ',err);
        return resolve('Cannot get device name');
      }
      var arr = stdout.split('\r\n');
      for(var i=0;i<arr.length;i++){
        var s = arr[i].trim();
        if( s && s!='FullName' ){
          return resolve(s);
        }
      }
      return resolve('No device name');
    });
  });
}

function get_route_list(){
  return check_exist(my.config.route_dir)
  .then((ret)=>{
    if( ret ){
      return fs.promises.readdir( my.config.route_dir, {withFileTypes: true} )
      .then((files)=>{
        return Promise.resolve(files.filter(f => f.isDirectory()).map(f => f.name));
      });
    }
    else{
      return Promise.resolve([]);
    }
  });
}

function check_exist(src){
  return fs.promises.access( src, fs.constants.F_OK )
  .then(()=>{
    return Promise.resolve(true);
  })
  .catch((e)=>{
    if( e.code === "ENOENT")
      return Promise.resolve(false);
    else
      return Promise.reject(e);
  });
}

if (require.main === module) {
  init();
}
