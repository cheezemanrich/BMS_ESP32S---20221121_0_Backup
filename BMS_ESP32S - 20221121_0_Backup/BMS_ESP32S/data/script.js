

function parentWidth(elem) {
  return elem.parentElement.clientWidth;
}

function parentHeight(elem) {
  return elem.parentElement.clientHeight;
}

// Create events for the readings
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
	
	var batt1highcell = 0;
	var batt2highcell = 0;
	var batt3highcell = 0;
	var batt4highcell = 0;
	var batt5highcell = 0;
	var batt6highcell = 0;
	var batt7highcell = 0;
	var batt8highcell = 0;
	var batt1lowcell = 0;
	var batt2lowcell = 0;
	var batt3lowcell = 0;
	var batt4lowcell = 0;
	var batt5lowcell = 0;
	var batt6lowcell = 0;
	var batt7lowcell = 0;
	var batt8lowcell = 0;
	var batt1hightemp = 0;
	var batt2hightemp = 0;
	var batt3hightemp = 0;
	var batt4hightemp = 0;
	var batt5hightemp = 0;
	var batt6hightemp = 0;
	var batt7hightemp = 0;
	var batt8hightemp = 0;
	var uppercurrent = 0;
	var lowercurrent = 0;
	var totalcurrent = 0;
	var uppervoltage = 0;
	var lowervoltage = 0;
	var totalvoltage = 0;
	var totalsoc = 0;
	

	// Calc highest cell volt
	batt1highcell = obj.batt1data1;
	if (obj.batt1data2 >= batt1highcell) {batt1highcell = obj.batt1data2};
	if (obj.batt1data3 >= batt1highcell) {batt1highcell = obj.batt1data3};
	if (obj.batt1data4 >= batt1highcell) {batt1highcell = obj.batt1data4};

	batt2highcell = obj.batt2data1;
	if (obj.batt2data2 >= batt2highcell) {batt2highcell = obj.batt2data2};
	if (obj.batt2data3 >= batt2highcell) {batt2highcell = obj.batt2data3};
	if (obj.batt2data4 >= batt2highcell) {batt2highcell = obj.batt2data4};

	batt3highcell = obj.batt3data1;
	if (obj.batt3data2 >= batt3highcell) {batt3highcell = obj.batt3data2};
	if (obj.batt3data3 >= batt3highcell) {batt3highcell = obj.batt3data3};
	if (obj.batt3data4 >= batt3highcell) {batt3highcell = obj.batt3data4};

	batt4highcell = obj.batt4data1;
	if (obj.batt4data2 >= batt4highcell) {batt4highcell = obj.batt4data2};
	if (obj.batt4data3 >= batt4highcell) {batt4highcell = obj.batt4data3};
	if (obj.batt4data4 >= batt4highcell) {batt4highcell = obj.batt4data4};
	
	batt5highcell = obj.batt5data1;
	if (obj.batt5data2 >= batt5highcell) {batt5highcell = obj.batt5data2};
	if (obj.batt5data3 >= batt5highcell) {batt5highcell = obj.batt5data3};
	if (obj.batt5data4 >= batt5highcell) {batt5highcell = obj.batt5data4};
	
	batt6highcell = obj.batt6data1;
	if (obj.batt6data2 >= batt6highcell) {batt6highcell = obj.batt6data2};
	if (obj.batt6data3 >= batt6highcell) {batt6highcell = obj.batt6data3};
	if (obj.batt6data4 >= batt6highcell) {batt6highcell = obj.batt6data4};
	
	batt7highcell = obj.batt7data1;
	if (obj.batt7data2 >= batt7highcell) {batt7highcell = obj.batt7data2};
	if (obj.batt7data3 >= batt7highcell) {batt7highcell = obj.batt7data3};
	if (obj.batt7data4 >= batt7highcell) {batt7highcell = obj.batt7data4};
	
	batt8highcell = obj.batt8data1;
	if (obj.batt8data2 >= batt8highcell) {batt8highcell = obj.batt8data2};
	if (obj.batt8data3 >= batt8highcell) {batt8highcell = obj.batt8data3};
	if (obj.batt8data4 >= batt8highcell) {batt8highcell = obj.batt8data4};

	//Calc lowest cell volt
	batt1lowcell = obj.batt1data1;
	if (obj.batt1data2 <= batt1lowcell) {batt1lowcell = obj.batt1data2};
	if (obj.batt1data3 <= batt1lowcell) {batt1lowcell = obj.batt1data3};
	if (obj.batt1data4 <= batt1lowcell) {batt1lowcell = obj.batt1data4};

	batt2lowcell = obj.batt2data1;
	if (obj.batt2data2 <= batt2lowcell) {batt2lowcell = obj.batt2data2};
	if (obj.batt2data3 <= batt2lowcell) {batt2lowcell = obj.batt2data3};
	if (obj.batt2data4 <= batt2lowcell) {batt2lowcell = obj.batt2data4};

	batt3lowcell = obj.batt3data1;
	if (obj.batt3data2 <= batt3lowcell) {batt3lowcell = obj.batt3data2};
	if (obj.batt3data3 <= batt3lowcell) {batt3lowcell = obj.batt3data3};
	if (obj.batt3data4 <= batt3lowcell) {batt3lowcell = obj.batt3data4};

	batt4lowcell = obj.batt4data1;
	if (obj.batt4data2 <= batt4lowcell) {batt4lowcell = obj.batt4data2};
	if (obj.batt4data3 <= batt4lowcell) {batt4lowcell = obj.batt4data3};
	if (obj.batt4data4 <= batt4lowcell) {batt4lowcell = obj.batt4data4};
	
	batt5lowcell = obj.batt5data1;
	if (obj.batt5data2 <= batt5lowcell) {batt5lowcell = obj.batt5data2};
	if (obj.batt5data3 <= batt5lowcell) {batt5lowcell = obj.batt5data3};
	if (obj.batt5data4 <= batt5lowcell) {batt5lowcell = obj.batt5data4};
	
	batt6lowcell = obj.batt6data1;
	if (obj.batt6data2 <= batt6lowcell) {batt6lowcell = obj.batt6data2};
	if (obj.batt6data3 <= batt6lowcell) {batt6lowcell = obj.batt6data3};
	if (obj.batt6data4 <= batt6lowcell) {batt6lowcell = obj.batt6data4};
	
	batt7lowcell = obj.batt7data1;
	if (obj.batt7data2 <= batt7lowcell) {batt7lowcell = obj.batt7data2};
	if (obj.batt7data3 <= batt7lowcell) {batt7lowcell = obj.batt7data3};
	if (obj.batt7data4 <= batt7lowcell) {batt7lowcell = obj.batt7data4};
	
	batt8lowcell = obj.batt8data1;
	if (obj.batt8data2 <= batt8lowcell) {batt8lowcell = obj.batt8data2};
	if (obj.batt8data3 <= batt8lowcell) {batt8lowcell = obj.batt8data3};
	if (obj.batt8data4 <= batt8lowcell) {batt8lowcell = obj.batt8data4};

	// Calc highest cell temp
	batt1hightemp = obj.batt1data6;
	if (obj.batt1data7 >= batt1hightemp) {batt1hightemp = obj.batt1data7};
	if (obj.batt1data8 >= batt1hightemp) {batt1hightemp = obj.batt1data8};
	if (obj.batt1data9 >= batt1hightemp) {batt1hightemp = obj.batt1data9};

	batt2hightemp = obj.batt2data6;
	if (obj.batt2data7 >= batt2hightemp) {batt2hightemp = obj.batt2data7};
	if (obj.batt2data8 >= batt2hightemp) {batt2hightemp = obj.batt2data8};
	if (obj.batt2data9 >= batt2hightemp) {batt2hightemp = obj.batt2data9};

	batt3hightemp = obj.batt3data6;
	if (obj.batt3data7 >= batt3hightemp) {batt3hightemp = obj.batt3data7};
	if (obj.batt3data8 >= batt3hightemp) {batt3hightemp = obj.batt3data8};
	if (obj.batt3data9 >= batt3hightemp) {batt3hightemp = obj.batt3data9};

	batt4hightemp = obj.batt4data6;
	if (obj.batt4data7 >= batt4hightemp) {batt4hightemp = obj.batt4data7};
	if (obj.batt4data8 >= batt4hightemp) {batt4hightemp = obj.batt4data8};
	if (obj.batt4data9 >= batt4hightemp) {batt4hightemp = obj.batt4data9};
	
	batt5hightemp = obj.batt5data6;
	if (obj.batt5data7 >= batt5hightemp) {batt5hightemp = obj.batt5data7};
	if (obj.batt5data8 >= batt5hightemp) {batt5hightemp = obj.batt5data8};
	if (obj.batt5data9 >= batt5hightemp) {batt5hightemp = obj.batt5data9};
	
	batt6hightemp = obj.batt6data6;
	if (obj.batt6data7 >= batt6hightemp) {batt6hightemp = obj.batt6data7};
	if (obj.batt6data8 >= batt6hightemp) {batt6hightemp = obj.batt6data8};
	if (obj.batt6data9 >= batt6hightemp) {batt6hightemp = obj.batt6data9};
	
	batt7hightemp = obj.batt7data6;
	if (obj.batt7data7 >= batt7hightemp) {batt7hightemp = obj.batt7data7};
	if (obj.batt7data8 >= batt7hightemp) {batt7hightemp = obj.batt7data8};
	if (obj.batt7data9 >= batt7hightemp) {batt7hightemp = obj.batt7data9};
	
	batt8hightemp = obj.batt8data6;
	if (obj.batt8data7 >= batt8hightemp) {batt8hightemp = obj.batt8data7};
	if (obj.batt8data8 >= batt8hightemp) {batt8hightemp = obj.batt8data8};
	if (obj.batt8data9 >= batt8hightemp) {batt8hightemp = obj.batt8data9};
	
	// Calc current voltage and soc totals
	uppercurrent = (Number(obj.batt1data13) + Number(obj.batt2data13) + Number(obj.batt7data13) + Number(obj.batt8data13));	// total of 4 upper batt currents
	lowercurrent = (Number(obj.batt3data13) + Number(obj.batt4data13) + Number(obj.batt5data13) + Number(obj.batt6data13));	// total of 4 lower batt currents
	totalcurrent = (uppercurrent + lowercurrent) /2;	// average of upper and lower current
	uppervoltage = ((Number(obj.batt1data5) + Number(obj.batt2data5) + Number(obj.batt7data5) + Number(obj.batt8data5)) / 4);	// average of 4 upper batt voltages
	lowervoltage = ((Number(obj.batt3data5) + Number(obj.batt4data5) + Number(obj.batt5data5) + Number(obj.batt6data5)) / 4);	// average of 4 lower batt voltages
	totalvoltage = uppervoltage + lowervoltage; // total of upper and lower voltages (system voltage)
		// average of all 8 batt SOC readings
	totalsoc = (Number(obj.batt1data15) + Number(obj.batt2data15) + Number(obj.batt3data15) + Number(obj.batt4data15) + Number(obj.batt5data15) + Number(obj.batt6data15) + Number(obj.batt7data15) + Number(obj.batt8data15)) / 8;
	
	// Graph button
	document.getElementById("graphbutton").innerHTML = "<button onclick=\"document.location='graph.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Graph</h3></button>"

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
	if(Number(obj.batt4data11 == 0) && Number(obj.batt5data12 == 0)) {document.getElementById("battery5button").innerHTML = "<button onclick=\"document.location='batt5.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Battery 5</h3></button>"} 
	else {document.getElementById("battery5button").innerHTML = "<button onclick=\"document.location='batt5.html'\"><h3 style=\"background-color:Tomato;\">Battery 5</h3></button>"};
	// Battery 6 Button Status color
	if(Number(obj.batt6data11 == 0) && Number(obj.batt6data12 == 0)) {document.getElementById("battery6button").innerHTML = "<button onclick=\"document.location='batt6.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Battery 6</h3></button>"} 
	else {document.getElementById("battery6button").innerHTML = "<button onclick=\"document.location='batt6.html'\"><h3 style=\"background-color:Tomato;\">Battery 6</h3></button>"};
	// Battery 7 Button Status color
	if(Number(obj.batt7data11 == 0) && Number(obj.batt7data12 == 0)) {document.getElementById("battery7button").innerHTML = "<button onclick=\"document.location='batt7.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Battery 7</h3></button>"} 
	else {document.getElementById("battery7button").innerHTML = "<button onclick=\"document.location='batt7.html'\"><h3 style=\"background-color:Tomato;\">Battery 7</h3></button>"};
	// Battery 8 Button Status color
	if(Number(obj.batt8data11 == 0) && Number(obj.batt8data12 == 0)) {document.getElementById("battery8button").innerHTML = "<button onclick=\"document.location='batt8.html'\"><h3 style=\"background-color:MediumSeaGreen;\">Battery 8</h3></button>"} 
	else {document.getElementById("battery8button").innerHTML = "<button onclick=\"document.location='batt8.html'\"><h3 style=\"background-color:Tomato;\">Battery 8</h3></button>"};
	
	// Title System Status color
	if(Number(obj.batt1data11 == 0) && Number(obj.batt2data11 == 0) && Number(obj.batt3data11 == 0) && Number(obj.batt4data11 == 0) && Number(obj.batt1data12 == 0) && Number(obj.batt2data12 == 0) && Number(obj.batt3data12 == 0) && Number(obj.batt4data12 == 0)) {document.getElementById("systemcolor").innerHTML = "<h3 style=\"background-color:MediumSeaGreen;\"><b>System</b></h3>"} 
	else {document.getElementById("systemcolor").innerHTML = "<h3 style=\"background-color:Tomato;\"><b>System</b></h3>"};
	
	document.getElementById("systemsoc").innerHTML = (totalsoc / 10).toFixed(1);
	document.getElementById("systemvolttotal").innerHTML = (totalvoltage / 1000).toFixed(3); // change mV to V
	document.getElementById("systemvoltupper").innerHTML = (uppervoltage / 1000).toFixed(3); // change mV to V
	document.getElementById("systemvoltlower").innerHTML = (lowervoltage / 1000).toFixed(3); // change mV to V
	document.getElementById("systemcurrent").innerHTML = (totalcurrent / 100).toFixed(1);	// change to A
	document.getElementById("systemcurrupper").innerHTML = (uppercurrent / 100).toFixed(1);	// change to A
	document.getElementById("systemcurrlower").innerHTML = (lowercurrent / 100).toFixed(1);	// change to A
	document.getElementById("systempower").innerHTML = ((totalvoltage / 1000) * (totalcurrent / 100)).toFixed(0); // change to V, change to A
	document.getElementById("systemchargerelay").innerHTML = obj.battchrelay;
	document.getElementById("systemloadrelay").innerHTML = obj.battldrelay;
	
	if((totalcurrent / 100) > 0) {document.getElementById("systemstatus").innerHTML = "DISCHARGE"} else {document.getElementById("systemstatus").innerHTML = "CHARGE"};
	
	document.getElementById("delta1").innerHTML = Number(obj.batt1data16);
	document.getElementById("delta2").innerHTML = Number(obj.batt2data16);
	document.getElementById("delta3").innerHTML = Number(obj.batt3data16);
	document.getElementById("delta4").innerHTML = Number(obj.batt4data16);
	document.getElementById("delta5").innerHTML = Number(obj.batt5data16);
	document.getElementById("delta6").innerHTML = Number(obj.batt6data16);
	document.getElementById("delta7").innerHTML = Number(obj.batt7data16);
	document.getElementById("delta8").innerHTML = Number(obj.batt8data16);
	if(obj.batt1data14 == 1){document.getElementById("balance1").innerHTML = "<span style=\"background-color:yellow;\">BAL</span>"} else {document.getElementById("balance1").innerHTML = "OFF"};
	if(obj.batt2data14 == 1){document.getElementById("balance2").innerHTML = "<span style=\"background-color:yellow;\">BAL</span>"} else {document.getElementById("balance2").innerHTML = "OFF"};
	if(obj.batt3data14 == 1){document.getElementById("balance3").innerHTML = "<span style=\"background-color:yellow;\">BAL</span>"} else {document.getElementById("balance3").innerHTML = "OFF"};
	if(obj.batt4data14 == 1){document.getElementById("balance4").innerHTML = "<span style=\"background-color:yellow;\">BAL</span>"} else {document.getElementById("balance4").innerHTML = "OFF"};
	if(obj.batt5data14 == 1){document.getElementById("balance5").innerHTML = "<span style=\"background-color:yellow;\">BAL</span>"} else {document.getElementById("balance5").innerHTML = "OFF"};
	if(obj.batt6data14 == 1){document.getElementById("balance6").innerHTML = "<span style=\"background-color:yellow;\">BAL</span>"} else {document.getElementById("balance6").innerHTML = "OFF"};
	if(obj.batt7data14 == 1){document.getElementById("balance7").innerHTML = "<span style=\"background-color:yellow;\">BAL</span>"} else {document.getElementById("balance7").innerHTML = "OFF"};
	if(obj.batt8data14 == 1){document.getElementById("balance8").innerHTML = "<span style=\"background-color:yellow;\">BAL</span>"} else {document.getElementById("balance8").innerHTML = "OFF"};
	document.getElementById("batt1data5").innerHTML = (Number(obj.batt1data5) /1000).toFixed(3);	// Get total voltages
	document.getElementById("batt2data5").innerHTML = (Number(obj.batt2data5) /1000).toFixed(3);
	document.getElementById("batt3data5").innerHTML = (Number(obj.batt3data5) /1000).toFixed(3);
	document.getElementById("batt4data5").innerHTML = (Number(obj.batt4data5) /1000).toFixed(3);
	document.getElementById("batt5data5").innerHTML = (Number(obj.batt5data5) /1000).toFixed(3);
	document.getElementById("batt6data5").innerHTML = (Number(obj.batt6data5) /1000).toFixed(3);
	document.getElementById("batt7data5").innerHTML = (Number(obj.batt7data5) /1000).toFixed(3);
	document.getElementById("batt8data5").innerHTML = (Number(obj.batt8data5) /1000).toFixed(3);
	document.getElementById("batt1data13").innerHTML = (Number(obj.batt1data13) /100).toFixed(1);	// Get currents
	document.getElementById("batt2data13").innerHTML = (Number(obj.batt2data13) /100).toFixed(1);
	document.getElementById("batt3data13").innerHTML = (Number(obj.batt3data13) /100).toFixed(1);
	document.getElementById("batt4data13").innerHTML = (Number(obj.batt4data13) /100).toFixed(1);
	document.getElementById("batt5data13").innerHTML = (Number(obj.batt5data13) /100).toFixed(1);
	document.getElementById("batt6data13").innerHTML = (Number(obj.batt6data13) /100).toFixed(1);
	document.getElementById("batt7data13").innerHTML = (Number(obj.batt7data13) /100).toFixed(1);
	document.getElementById("batt8data13").innerHTML = (Number(obj.batt8data13) /100).toFixed(1);
	document.getElementById("batt1data15").innerHTML = Number(obj.batt1data15) /10;		//Get SOCs
	document.getElementById("batt2data15").innerHTML = Number(obj.batt2data15) /10;
	document.getElementById("batt3data15").innerHTML = Number(obj.batt3data15) /10;
	document.getElementById("batt4data15").innerHTML = Number(obj.batt4data15) /10;
	document.getElementById("batt5data15").innerHTML = Number(obj.batt5data15) /10;
	document.getElementById("batt6data15").innerHTML = Number(obj.batt6data15) /10;
	document.getElementById("batt7data15").innerHTML = Number(obj.batt7data15) /10;
	document.getElementById("batt8data15").innerHTML = Number(obj.batt8data15) /10;
	document.getElementById("batt1highcell").innerHTML = ((Number(batt1highcell)) / 1000).toFixed(3);
	document.getElementById("batt2highcell").innerHTML = ((Number(batt2highcell)) / 1000).toFixed(3);
	document.getElementById("batt3highcell").innerHTML = ((Number(batt3highcell)) / 1000).toFixed(3);
	document.getElementById("batt4highcell").innerHTML = ((Number(batt4highcell)) / 1000).toFixed(3);
	document.getElementById("batt5highcell").innerHTML = ((Number(batt5highcell)) / 1000).toFixed(3);
	document.getElementById("batt6highcell").innerHTML = ((Number(batt6highcell)) / 1000).toFixed(3);
	document.getElementById("batt7highcell").innerHTML = ((Number(batt7highcell)) / 1000).toFixed(3);
	document.getElementById("batt8highcell").innerHTML = ((Number(batt8highcell)) / 1000).toFixed(3);
	document.getElementById("batt1lowcell").innerHTML = ((Number(batt1lowcell)) / 1000).toFixed(3);
	document.getElementById("batt2lowcell").innerHTML = ((Number(batt2lowcell)) / 1000).toFixed(3);
	document.getElementById("batt3lowcell").innerHTML = ((Number(batt3lowcell)) / 1000).toFixed(3);
	document.getElementById("batt4lowcell").innerHTML = ((Number(batt4lowcell)) / 1000).toFixed(3);
	document.getElementById("batt5lowcell").innerHTML = ((Number(batt5lowcell)) / 1000).toFixed(3);
	document.getElementById("batt6lowcell").innerHTML = ((Number(batt6lowcell)) / 1000).toFixed(3);
	document.getElementById("batt7lowcell").innerHTML = ((Number(batt7lowcell)) / 1000).toFixed(3);
	document.getElementById("batt8lowcell").innerHTML = ((Number(batt8lowcell)) / 1000).toFixed(3);
	document.getElementById("batt1hightemp").innerHTML = ((Number(batt1hightemp)) / 100).toFixed(1);
	document.getElementById("batt2hightemp").innerHTML = ((Number(batt2hightemp)) / 100).toFixed(1);
	document.getElementById("batt3hightemp").innerHTML = ((Number(batt3hightemp)) / 100).toFixed(1);
	document.getElementById("batt4hightemp").innerHTML = ((Number(batt4hightemp)) / 100).toFixed(1);
	document.getElementById("batt5hightemp").innerHTML = ((Number(batt5hightemp)) / 100).toFixed(1);
	document.getElementById("batt6hightemp").innerHTML = ((Number(batt6hightemp)) / 100).toFixed(1);
	document.getElementById("batt7hightemp").innerHTML = ((Number(batt7hightemp)) / 100).toFixed(1);
	document.getElementById("batt8hightemp").innerHTML = ((Number(batt8hightemp)) / 100).toFixed(1);
	
  }, false);

}

