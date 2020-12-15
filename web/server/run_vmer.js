'use strict';

const PORT = 8080;

const Promise = require('bluebird');
const express = require('express');
var http = require('http');
var app = express();
var escape = require('escape-html');
const bodyParser = require('body-parser');
var PATH = require('path');
var static_path = PATH.resolve( '../static' );
var db = require('./db_mysql');

var server = http.createServer(function (req, res) {
  app(req,res);
});

function init(){
  return db.init()
  .then(()=>{
    return db.get_connection();
  }).then((conn)=>{
    init_web();
    server.on('error', function (e) {
      // Handle your error here
    });
    server.listen( PORT, '127.0.0.1');
  });
}


function init_web(){
  app.set('port', PORT);
  app.use('/js',express.static(PATH.join(static_path, '/js')));
  app.use('/css', express.static(PATH.join(static_path, '/css')));
  app.use('/img', express.static( PATH.join(static_path, '/img')));
  app.use('/fonts', express.static( PATH.join(static_path, '/fonts')));

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
    process_api( req, res, req.params.api, data );
  });

  app.post('/api/:api', function(req,res,next){
    process_api( req, res, req.params.api, req.body );
  });

  app.get('/',function(req,res){
    res.redirect('/index.html');
  });
  app.get('/:page',function(req,res){
    process_page(req, res);
  });
}


// send error message back to client if error with API
function api_error(req, res, err){
  var user_id = req.session && req.session.user_id ? req.session.user_id : null;
  var msg = (err && typeof(err)=='string') ? err : 'Unknown error';
  res.send({
    result: 'fail',
    msg: msg
  });
  if( err )
    console.log('api error : ',err);
};


function process_api( req, res, api_name, data ){
  var conn;
  console.log('api : '+api_name, data);
  db.get_connection()
  .then((_conn)=>{
    conn = _conn;
    if( api_name=='get_factory_list' ){
      return db.get_factory_list( conn )
      .then((ret)=>{
        res.send({
          result: 'success',
          factories: ret
        });
      });
    }
    else if( api_name=='get_route_list' ){
      return db.get_route_list( conn, data.factory_id )
      .then((ret)=>{
        res.send({
          result: 'success',
          routes: ret
        });
      });
    }
    else if( api_name=='get_route' ){
      return db.get_route( conn, data.route_id )
      .then((ret)=>{
        res.send({
          result: 'success',
          route: ret
        });
      });
    }
    else{
      api_error(req, res, 'invalid api : '+api_name);
    }
  })
  .catch((err)=>{
    if( err )
      api_error(req, res, err);
  })
  .finally(()=>{
    db.release_connection(conn);
  });
}

function process_page(req, res){
  const STATIC_PAGES = ['index.html','404.html','favicon.ico'];
  if( STATIC_PAGES.includes(req.params.page) ){
    res.sendFile( PATH.join( static_path, '/' + req.params.page), (e)=>{
      if( e && req.params.page!='404.html' ){
        res.redirect('/404.html');
      }
    });
  }
  else{
    res.redirect('/404.html');
  }
}


if (require.main === module) {
  init();
  console.log('running on port : '+PORT);
}
