
function parentWidth(elem) {
  return elem.parentElement.clientWidth;
}

function parentHeight(elem) {
  return elem.parentElement.clientHeight;
}

// Create events for the sensor readings
if (!!window.EventSource) {
  var source = new EventSource('/events');

  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('new_readings', function(e) {
    var obj = JSON.parse(e.data);
	
	// System Button Status color
	if(Number(obj.batt1data11 == 0) && Number(obj.batt2data11 == 0) && Number(obj.batt3data11 == 0) && Number(obj.batt4data11 == 0) && Number(obj.batt1data12 == 0) && Number(obj.batt2data12 == 0) && Number(obj.batt3data12 == 0) && Number(obj.batt4data12 == 0)) {document.getElementById("systembutton").innerHTML = "<button onclick=\"document.location='index.html'\"><h3 style=\"background-color:MediumSeaGreen;\">System</h3></button>"} 
	else {document.getElementById("systembutton").innerHTML = "<button onclick=\"document.location='index.html'\"><h3 style=\"background-color:Tomato;\">System</h3></button>"};
	
	// Battery 1 Button Status color
	if(Number(obj.batt1data11 == 0) && Number(obj.batt1data12 == 0)) {document.getElementById("battery1button").innerHTML = "<button onclick=\"document.location='batt1.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Battery 1</h3></button>"} 
	else {document.getElementById("battery1button").innerHTML = "<button onclick=\"document.location='batt1.html'\"><h3 style=\"background-color:Tomato;\">Battery 1</h3></button>"};
	
	// Battery 2 Button Status color
	if(Number(obj.batt2data11 == 0) && Number(obj.batt2data12 == 0)) {document.getElementById("battery2button").innerHTML = "<button onclick=\"document.location='batt2.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Battery 2</h3></button>"} 
	else {document.getElementById("battery2button").innerHTML = "<button onclick=\"document.location='batt2.html'\"><h3 style=\"background-color:Tomato;\">Battery 2</h3></button>"};
	
	// Battery 3 Button Status color
	if(Number(obj.batt3data11 == 0) && Number(obj.batt3data12 == 0)) {document.getElementById("battery3button").innerHTML = "<button onclick=\"document.location='batt3.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Battery 3</h3></button>"} 
	else {document.getElementById("battery3button").innerHTML = "<button onclick=\"document.location='batt3.html'\"><h3 style=\"background-color:Tomato;\">Battery 3</h3></button>"};
	
	// Battery 4 Button Status color
	if(Number(obj.batt4data11 == 0) && Number(obj.batt4data12 == 0)) {document.getElementById("battery4button").innerHTML = "<button onclick=\"document.location='batt4.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Battery 4</h3></button>"} 
	else {document.getElementById("battery4button").innerHTML = "<button onclick=\"document.location='batt4.html'\"><h3 style=\"background-color:Tomato;\">Battery 4</h3></button>"};
	
	// Battery 5 Button Status color
	if(Number(obj.batt5data11 == 0) && Number(obj.batt5data12 == 0)) {document.getElementById("battery5button").innerHTML = "<button onclick=\"document.location='batt5.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Battery 5</h3></button>"} 
	else {document.getElementById("battery5button").innerHTML = "<button onclick=\"document.location='batt5.html'\"><h3 style=\"background-color:Tomato;\">Battery 5</h3></button>"};
	
	// Battery 6 Button Status color
	if(Number(obj.batt6data11 == 0) && Number(obj.batt6data12 == 0)) {document.getElementById("battery6button").innerHTML = "<button onclick=\"document.location='batt6.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Battery 6</h3></button>"} 
	else {document.getElementById("battery6button").innerHTML = "<button onclick=\"document.location='batt6.html'\"><h3 style=\"background-color:Tomato;\">Battery 6</h3></button>"};
	
	// Battery 7 Button Status color
	if(Number(obj.batt7data11 == 0) && Number(obj.batt7data12 == 0)) {document.getElementById("battery7button").innerHTML = "<button onclick=\"document.location='batt7.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Battery 7</h3></button>"} 
	else {document.getElementById("battery7button").innerHTML = "<button onclick=\"document.location='batt7.html'\"><h3 style=\"background-color:Tomato;\">Battery 7</h3></button>"};
	
	if(Number(obj.batt8data11 == 0) && Number(obj.batt8data12 == 0)) {document.getElementById("systemcolor").innerHTML = "<h3 style=\"background-color:MediumSeaGreen;\"><b>Battery 8</b></h3>"} else {document.getElementById("systemcolor").innerHTML = "<h3 style=\"background-color:Tomato;\"><b>Battery 8</b></h3>"};
	document.getElementById("batt8data1").innerHTML = (Number(obj.batt8data1) /1000).toFixed(3);
    document.getElementById("batt8data2").innerHTML = (Number(obj.batt8data2) /1000).toFixed(3);
    document.getElementById("batt8data3").innerHTML = (Number(obj.batt8data3) /1000).toFixed(3);
	document.getElementById("batt8data4").innerHTML = (Number(obj.batt8data4) /1000).toFixed(3);
	document.getElementById("batt8data5").innerHTML = (Number(obj.batt8data5) /1000).toFixed(3);
	document.getElementById("batt8data6").innerHTML = (Number(obj.batt8data6) /100).toFixed(1);
    document.getElementById("batt8data7").innerHTML = (Number(obj.batt8data7) /100).toFixed(1);
    document.getElementById("batt8data8").innerHTML = (Number(obj.batt8data8) /100).toFixed(1);
	document.getElementById("batt8data9").innerHTML = (Number(obj.batt8data9) /100).toFixed(1);
	document.getElementById("batt8data10").innerHTML = (Number(obj.batt8data10) /100).toFixed(1);
	if(Number(obj.batt8data11 == 0)){document.getElementById("batt8data11").innerHTML = "OK"} else {document.getElementById("batt8data11").innerHTML = "WARNING"};
	if(Number(obj.batt8data12 == 0)){document.getElementById("batt8data12").innerHTML = "OK"} else {document.getElementById("batt8data12").innerHTML = "WARNING"};
	document.getElementById("batt8data13").innerHTML = (Number(obj.batt8data13) /100).toFixed(1);
	if(obj.batt8data14 == 1){document.getElementById("batt8data14").innerHTML = "BALANCE"} else {document.getElementById("batt8data14").innerHTML = "OFF"};
	document.getElementById("batt8data15").innerHTML = Number(obj.batt8data15) /10;
	document.getElementById("delta8").innerHTML = Number(obj.batt8data16);
  }, false);

}

