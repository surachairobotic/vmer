'use strict';


function escapeHtml(unsafe) {
  return unsafe
  .replace(/&/g, "&amp;")
  .replace(/</g, "&lt;")
  .replace(/>/g, "&gt;")
  .replace(/"/g, "&quot;")
  .replace(/'/g, "&#039;");
}

const Days = [
  'Sunday',
  'Monday',
  'Tuesday',
  'Wednesday',
  'Thursday',
  'Friday',
  'Saturday',
];

function myapi( api_name, data, callback ){
  return $.get('/api/'+api_name, data ? {data: data} : null)
  .done((ret)=>{
    if( ret.result=='success' ){
      callback(ret);
    }
    else{
      console.error(api_name+' failed : '+ret.msg);
    }
  })
  .fail(()=>{
    console.error(api_name+' failed');
  });
}

function myFunction() {
  document.getElementById("demo").style.color = "red";
  console.error('test : Click me to change my text color.');
  
}

$(function(){
  myapi( 'get_factory_list', null, (ret)=>{
    var s = '<option value="" disabled="disabled" selected>- กรุณาเลือก -</option>';
    ret.factories.forEach((f)=>{
      s+= '<option value="'+f.id+'">'+escapeHtml(f.name)+'</option>';
    });
    $('#select_factory').html(s);
  });
  
  $('#select_factory').on('change', function(){
    var factory_id = parseInt(this.value);
    myapi( 'get_route_list', {factory_id: factory_id}, (ret)=>{
      var s = '<option value="" disabled="disabled" selected>- กรุณาเลือก -</option>';
      ret.routes.forEach((r)=>{
        s+= '<option value="'+r.id+'">'+escapeHtml(r.name)+'</option>';
      });
      $('#select_route').html(s);
    });
  });
  
  $('#select_route').on('change', function(){
    var route_id = parseInt(this.value);
    myapi( 'get_route', {route_id: route_id}, (ret)=>{
      var r = ret.route, s = '<p id="route_day">Day : '+Days[r.day]+'</p>';
      r.machines.forEach((m)=>{
        s+= '<p class="machine_name">Machine : '+escapeHtml(m.name)+'</p>';
      });
      $('#div_route').html( s );
    });
  });
  
  $('#btnDemo').click(function(){
    myapi( 'get_factory_list', null, (ret)=>{
      $('#p1').text(JSON.stringify(ret))
    });
  });

  $("#uploadform").submit(function( event ) {
    alert( "Handler for .submit() called." );
    event.preventDefault();
    console.log($('#fname').val());
    console.log($('#detail').val());
    console.log($('#fimage').val());
    myapi( 'upload', {fname:$('#fname').val(), detail:$('#detail').val(), image:$('#fimage').val()}, (ret)=>{
      $('#p1').text(JSON.stringify(ret))
    });
  });
});
