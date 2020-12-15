'use strict';

const mysql = require('promise-mysql');
var fs = require('fs');
const Promise = require('bluebird');
var PATH = require('path');
var pool = null;


module.exports = {
  init: function(){
    return new Promise(function(resolve, reject){
      var conn;
      mysql.createPool({
        connectionLimit : 16,
        host: 'localhost',
        user: 'vmer',
        password: 'Surachai!1234',
        database: 'vmer'
      }).then((_pool)=>{
        pool = _pool;
        return pool.getConnection();
      }).then((_conn)=>{
        conn = _conn;
        pool.releaseConnection(_conn);
        resolve();
      }).catch(function(err) {
        console.log( 'creating pool failed : Try to restart.', err );
        setTimeout(()=>{
          process.exit(1);
          reject(err);
        }, 10000);
      });
    });
  },

  get_connection: function(){
    return pool.getConnection().then(function(connection) {
      return Promise.resolve(connection);
    }).catch((err)=>{
      return Promise.reject(err);
    });
  },

  // have to call or the program will not exit
  end: function(){
    if( pool ){
      pool.end();
      pool = null;
    }
  },

  release_connection: function(conn){
    pool.releaseConnection(conn);
    conn.connection.removeAllListeners(); // prevent MaxListenersExceededWarning
  },
  
  get_factory_list: get_factory_list,
  get_route_list: get_route_list,
  get_route: get_route,
};

function get_factory_list( conn ){
  return conn.query('SELECT * FROM factory');
}

function get_route_list( conn, factory_id ){
  return conn.query('SELECT id,name FROM route WHERE factory_id=?', parseInt(factory_id))
  .then((rows)=>{
    if( rows.length==0 ){
      return Promise.reject('Invalid factory ID');
    }
    return Promise.resolve(rows);
  });
}

function get_route( conn, route_id ){
  var ret;
  route_id = parseInt(route_id);
  return conn.query('SELECT id,name,day FROM route WHERE id=?', route_id)
  .then((rows)=>{
    if( rows.length==0 ){
      return Promise.reject('Invalid route ID');
    }
    ret = rows[0];
    return conn.query('SELECT machine.id,machine.name FROM route_machine INNER JOIN machine ON route_machine.machine_id=machine.id WHERE route_machine.route_id=?', route_id);
  }).then((rows)=>{
    ret.machines = rows;
    return Promise.resolve(ret);
  });
}



