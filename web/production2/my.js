'use strict';

const fs = require('fs');
var stream = null;
var logfile_time = null;
var logfile_name = null;
const Promise = require('bluebird');
const del = require('del');
var child_process = require('child_process');
var PATH = require('path');

module.exports = {
  init: function(options = {}){
    logfile_time = get_time(this.Date());
    module.exports.config = {
      "port":8085,
      "port_udp":8086,
      "tmp_dir":PATH.resolve("./tmp/"),
      "log_dir":PATH.resolve("./log/"),
      "result_dir":PATH.resolve("C:/vmer/data/"),
      "route_dir":PATH.resolve("C:/vmer/route/"),
    };
    process.env.NODE_ENV = 'production';
    stream = fs.createWriteStream( get_logfile_name(logfile_time), {flags:'a'});
    module.exports.log('info','port = '+module.exports.config['port']);
    module.exports.log('info','log = '+logfile_name);
    return check_dir_exist(module.exports.config.log_dir)
    .then(()=>{
      return check_dir_exist(module.exports.config.result_dir);
    }).then(()=>{
      return check_dir_exist(module.exports.config.route_dir);
    }).then(()=>{
      return check_dir_exist(module.exports.config.tmp_dir);
    }).then(()=>{
      return remove_old_log_files();
    })
  },

  get_time: get_time,
  get_time_str: get_time_str,

  log: function(level, msg, msg2){
    var str = get_log_str(level, msg, msg2);
    console.log(str);
    if( !stream )
      return;
    stream.write( str + '\n');
  },

  end: function(){
    if( stream ){
      stream.end();
      stream = null;
    }
    if( timer_clear_cache )
      clearInterval(timer_clear_cache);
  },

  log_wait: function(level, cb, msg, msg2){
    if( !stream ){
      cb();
      return;
    }
    var str = get_log_str(level, msg, msg2);
    console.log(str);
    stream.write( str + '\n', cb);
  },

  log_sync: function(level, msg, msg2){
    if( stream ){
      stream.end();
      stream = null;
    }
    var str = get_log_str(level, msg, msg2);
    console.log(str);
    fs.appendFileSync( logfile_name, str + '\n' );
  },

  remove_old_log_files: remove_old_log_files,

  is_release: function(){
    return this.config.app_mode=='release';
  },

  check_dir_exist: check_dir_exist,
  get_logfile_name: get_logfile_name,
  create_random_string: create_random_string,

  isNaN: function(n){
    return n===null || typeof(n) == 'undefined' || isNaN(n) || n==='';
  },

  del: function(path, b_check_deleted = true){
    return new Promise((resolve,reject)=>{
      setTimeout(()=>{
        del( path, {force:true,dryRun:false})
        .then((eles)=>{
          if( b_check_deleted && eles.length==0 ){
            module.exports.log('warn', 'my.del : cannot remove : '+path);
            reject();
          }
          else
            resolve();
        });
      }, 100);
    });
  },

  move_file: function(src, dst){
    module.exports.log('info', 'move_file: '+src+' => '+dst);
    return fs.promises.copyFile( src, dst )
    .then(()=>{
      return fs.promises.unlink( src );
    });
  },

  copy_all: function(src, dst){
    return fs.promises.readdir(src, {withFileTypes: true})
    .then((files)=>{
      return Promise.all( files.map((f)=>{
        if( f.isFile() ){
          return fs.promises.copyFile( PATH.join(src,f.name), PATH.join(dst,f.name) );
        }
        else{
          return check_dir_exist( PATH.join(dst,f.name) )
          .then(()=>{
            return module.exports.copy_all( PATH.join(src,f.name), PATH.join(dst,f.name) );
          });
        }
      }));
    });
  },

  Date: function(param){
    return param ? new Date(param) : new Date();
  },

  alert: function(msg,caption='Alert', b_sync=false){
    var cmd = "powershell.exe", arg = [`Add-Type -AssemblyName PresentationCore,PresentationFramework;[System.Windows.MessageBox]::Show('${msg}','${caption}');`];
    if(b_sync)
      child_process.spawnSync( cmd, arg );
    else
      child_process.spawn( cmd, arg );
  }

};

function get_time(currentdate){
  return {
    year: currentdate.getFullYear()-2000,
    month: currentdate.getMonth() + 1,
    date: currentdate.getDate(),
    day: currentdate.getDay(),
    hour: currentdate.getHours(),
    minute: currentdate.getMinutes(),
    second: currentdate.getSeconds()
  };
}

function get_logfile_name(t=null){
  if( !t )
    t = logfile_time;
  logfile_name = module.exports.config.log_dir + String(t.year) + (t.month<10 ? '0' : '') + t.month
    + (t.date < 10 ? ('0' + t.date) : t.date)
    + ".log";
  return logfile_name;
}


function get_time_str(t){
  return String(t.year)
    + (t.month<10 ? '0' : '') + t.month
    + (t.date < 10 ? ('0' + t.date) : t.date) + '-'
    + (t.hour < 10 ? ('0' + t.hour) : t.hour)
    + (t.minute < 10 ? ('0' + t.minute) : t.minute)
    + (t.second < 10 ? ('0' + t.second) : t.second);
}



function get_log_str(level, msg, msg2){
  var t = get_time(module.exports.Date());

  var fn = function(s){
    if( !s )
      return '';
    if( typeof(s)=='string' )
      return s;
    if( s.toString )
      return s.toString().replace('\\n', '\n');
    return JSON.stringify(s);
  };
  var e = '';
  if( level=='error' || level=='warn' ){
    e+='\n';
    if( msg && msg.stack )
      e+= msg.stack;
    else if( msg2 && msg2.stack )
      e+= msg2.stack;
    else if( level=='error' )
      e+= (new Error()).stack;
  }
  return get_time_str(t)
    + ',' + level + ','
    + fn(msg)
    + fn(msg2)
    + e;
}

function remove_old_log_files(){
  const MAX_FILE_NUM = 7;
  return fs.promises.readdir(module.exports.config.log_dir)
  .then((files)=>{
    if( files.length>MAX_FILE_NUM ){
      files = files.map((f)=>PATH.join(module.exports.config.log_dir,f));
      files.sort(function(a, b) {
        return fs.statSync(a).mtime.getTime() -
          fs.statSync(b).mtime.getTime();
      });
      for(var i=0;i<files.length-MAX_FILE_NUM;i++){
        fs.promises.unlink( files[i] );
      }
    }
  });
}


function check_dir_exist(dir){
  return fs.promises.access( dir, fs.constants.F_OK )
  .catch((e)=>{
    if( e.code === "ENOENT"){
      return fs.promises.mkdir( dir )
      .then(()=>{
        module.exports.log('info', 'dir created : '+dir);
      });
    }
    else{
      module.exports.log('error', 'check_dir_exist() : fs.access failed '+dir, e);
      return Promise.reject(e);
    }
  });
}


function create_random_string(len){
  const possible = "abcdefghijklmnpqrstuvwxyz123456789";
  var text = "";
  for (var i = 0; i < len; i++)
    text += possible.charAt(Math.floor(Math.random() * possible.length));
  return text;
}


if (require.main === module) {
  module.exports.init();
}
