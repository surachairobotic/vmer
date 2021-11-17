"use strict";var fs=require("fs");const Promise=require("bluebird");var my,PATH=require("path");(module.exports={load:function(e){my=e;var r;return Promise.all(["./config/common.txt","./config/individual.txt"].map((e=>{return r=e,fs.promises.readFile(r,"utf8").then((e=>{try{return Promise.resolve(JSON.parse(e))}catch(e){return my.log("warn","config.load : cannot parse config file : "+r+"\n",e),Promise.reject("Cannot parse config file : \n"+PATH.resolve(r)+"\n"+e.message)}})).catch((e=>"string"!=typeof e?Promise.reject("Cannot load config file : "+PATH.resolve(r)):Promise.reject(e)));var r}))).then((e=>{r=e[0];for(var o=1;o<e.length;o++)r={...r,...e[o]};var i=Object.keys(r),t=[];return i.forEach((e=>{e.indexOf("_dir")>=0&&(r[e]=PATH.resolve(r[e])+"/",t.push(r[e]))})),Promise.all(t.map((e=>my.check_dir_exist(e))))})).then((()=>Promise.resolve(r)))}},require.main===module)&&(my=require("./my")).init().then((()=>module.exports.load(my))).then((e=>{console.log(e),process.exit()}));