
window.addEventListener('load', getReadings);

// Create Voltage Chart
var chartT = new Highcharts.Chart({
  chart:{
    renderTo:'chart-Voltage'
  },
  series: [
    {
      name: 'Bat 1, Cel 1',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
    {
      name: 'Bat 1, Cel 2',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'square',
        radius: 3,
        fillColor: '#00A6A6',
      }
    },
    {
      name: 'Bat 1, Cel 3',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#8B2635',
      }
    },
    {
      name: 'Bat 1, Cel 4',
      type: 'line',
      color: '#71B48D',
      marker: {
        symbol: 'triangle-down',
        radius: 3,
        fillColor: '#71B48D',
      }
    },
	{
      name: 'Bat 2, Cel 1',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
    {
      name: 'Bat 2, Cel 2',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'square',
        radius: 3,
        fillColor: '#00A6A6',
      }
    },
    {
      name: 'Bat 2, Cel 3',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#8B2635',
      }
    },
    {
      name: 'Bat 2, Cel 4',
      type: 'line',
      color: '#71B48D',
      marker: {
        symbol: 'triangle-down',
        radius: 3,
        fillColor: '#71B48D',
      }
    },
	{
      name: 'Bat 3, Cel 1',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
    {
      name: 'Bat 3, Cel 2',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'square',
        radius: 3,
        fillColor: '#00A6A6',
      }
    },
    {
      name: 'Bat 3, Cel 3',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#8B2635',
      }
    },
    {
      name: 'Bat 3, Cel 4',
      type: 'line',
      color: '#71B48D',
      marker: {
        symbol: 'triangle-down',
        radius: 3,
        fillColor: '#71B48D',
      }
    },
	{
      name: 'Bat 4, Cel 1',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
    {
      name: 'Bat 4, Cel 2',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'square',
        radius: 3,
        fillColor: '#00A6A6',
      }
    },
    {
      name: 'Bat 4, Cel 3',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#8B2635',
      }
    },
    {
      name: 'Bat 4, Cel 4',
      type: 'line',
      color: '#71B48D',
      marker: {
        symbol: 'triangle-down',
        radius: 3,
        fillColor: '#71B48D',
      }
    },
	{
      name: 'Bat 5, Cel 1',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
    {
      name: 'Bat 5, Cel 2',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'square',
        radius: 3,
        fillColor: '#00A6A6',
      }
    },
    {
      name: 'Bat 5, Cel 3',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#8B2635',
      }
    },
    {
      name: 'Bat 5, Cel 4',
      type: 'line',
      color: '#71B48D',
      marker: {
        symbol: 'triangle-down',
        radius: 3,
        fillColor: '#71B48D',
      }
    },
	{
      name: 'Bat 6, Cel 1',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
    {
      name: 'Bat 6, Cel 2',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'square',
        radius: 3,
        fillColor: '#00A6A6',
      }
    },
    {
      name: 'Bat 6, Cel 3',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#8B2635',
      }
    },
    {
      name: 'Bat 6, Cel 4',
      type: 'line',
      color: '#71B48D',
      marker: {
        symbol: 'triangle-down',
        radius: 3,
        fillColor: '#71B48D',
      }
    },
	{
      name: 'Bat 7, Cel 1',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
    {
      name: 'Bat 7, Cel 2',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'square',
        radius: 3,
        fillColor: '#00A6A6',
      }
    },
    {
      name: 'Bat 7, Cel 3',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#8B2635',
      }
    },
    {
      name: 'Bat 7, Cel 4',
      type: 'line',
      color: '#71B48D',
      marker: {
        symbol: 'triangle-down',
        radius: 3,
        fillColor: '#71B48D',
      }
    },
	{
      name: 'Bat 8, Cel 1',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
    {
      name: 'Bat 8, Cel 2',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'square',
        radius: 3,
        fillColor: '#00A6A6',
      }
    },
    {
      name: 'Bat 8, Cel 3',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#8B2635',
      }
    },
    {
      name: 'Bat 8, Cel 4',
      type: 'line',
      color: '#71B48D',
      marker: {
        symbol: 'triangle-down',
        radius: 3,
        fillColor: '#71B48D',
      }
    },
  ],
  title: {
    text: undefined
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: {
      text: 'Voltage'
    }
  },
  credits: {
    enabled: false
  }
});



// Function to get current readings on the webpage when it loads for the first time
function getReadings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var obj = JSON.parse(this.responseText);
      console.log(obj);
      //plotVoltage(myObj);
    }
  };
  xhr.open("GET", "/readings", true);
  xhr.send();
}

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

  source.addEventListener('message', function(e) {
    console.log("message", e.data);
  }, false);

  source.addEventListener('new_readings', function(e) {
    //console.log("new_readings", e.data);
    var obj = JSON.parse(e.data);
    //console.log(obj);

	const MAXCHARTDATA = 400;
	var x = (new Date()).getTime();
	console.log("Date:");
	console.log(x);
	
	if(chartT.series[0].data.length > MAXCHARTDATA) {
      chartT.series[0].addPoint([x, Number(obj.batt1data1)], true, true, true);
    } else {
      chartT.series[0].addPoint([x, Number(obj.batt1data1)], true, false, true);
    }
	if(chartT.series[1].data.length > MAXCHARTDATA) {
      chartT.series[1].addPoint([x, Number(obj.batt1data2)], true, true, true);
    } else {
      chartT.series[1].addPoint([x, Number(obj.batt1data2)], true, false, true);
    }
	if(chartT.series[2].data.length > MAXCHARTDATA) {
      chartT.series[2].addPoint([x, Number(obj.batt1data3)], true, true, true);
    } else {
      chartT.series[2].addPoint([x, Number(obj.batt1data3)], true, false, true);
    }
	if(chartT.series[3].data.length > MAXCHARTDATA) {
      chartT.series[3].addPoint([x, Number(obj.batt1data4)], true, true, true);
    } else {
      chartT.series[3].addPoint([x, Number(obj.batt1data4)], true, false, true);
    }
	if(chartT.series[4].data.length > MAXCHARTDATA) {
      chartT.series[4].addPoint([x, Number(obj.batt2data1)], true, true, true);
    } else {
      chartT.series[4].addPoint([x, Number(obj.batt2data1)], true, false, true);
    }
	if(chartT.series[5].data.length > MAXCHARTDATA) {
      chartT.series[5].addPoint([x, Number(obj.batt2data2)], true, true, true);
    } else {
      chartT.series[5].addPoint([x, Number(obj.batt2data2)], true, false, true);
    }
	if(chartT.series[6].data.length > MAXCHARTDATA) {
      chartT.series[6].addPoint([x, Number(obj.batt2data3)], true, true, true);
    } else {
      chartT.series[6].addPoint([x, Number(obj.batt2data3)], true, false, true);
    }
	if(chartT.series[7].data.length > MAXCHARTDATA) {
      chartT.series[7].addPoint([x, Number(obj.batt2data4)], true, true, true);
    } else {
      chartT.series[7].addPoint([x, Number(obj.batt2data4)], true, false, true);
    }
	if(chartT.series[8].data.length > MAXCHARTDATA) {
      chartT.series[8].addPoint([x, Number(obj.batt3data1)], true, true, true);
    } else {
      chartT.series[8].addPoint([x, Number(obj.batt3data1)], true, false, true);
    }
	if(chartT.series[9].data.length > MAXCHARTDATA) {
      chartT.series[9].addPoint([x, Number(obj.batt3data1)], true, true, true);
    } else {
      chartT.series[9].addPoint([x, Number(obj.batt3data1)], true, false, true);
    }
	if(chartT.series[10].data.length > MAXCHARTDATA) {
      chartT.series[10].addPoint([x, Number(obj.batt3data1)], true, true, true);
    } else {
      chartT.series[10].addPoint([x, Number(obj.batt3data1)], true, false, true);
    }
	if(chartT.series[11].data.length > MAXCHARTDATA) {
      chartT.series[11].addPoint([x, Number(obj.batt3data1)], true, true, true);
    } else {
      chartT.series[11].addPoint([x, Number(obj.batt3data1)], true, false, true);
    }
	if(chartT.series[12].data.length > MAXCHARTDATA) {
      chartT.series[12].addPoint([x, Number(obj.batt4data1)], true, true, true);
    } else {
      chartT.series[12].addPoint([x, Number(obj.batt4data1)], true, false, true);
    }
	if(chartT.series[13].data.length > MAXCHARTDATA) {
      chartT.series[13].addPoint([x, Number(obj.batt4data2)], true, true, true);
    } else {
      chartT.series[13].addPoint([x, Number(obj.batt4data2)], true, false, true);
    }
	if(chartT.series[14].data.length > MAXCHARTDATA) {
      chartT.series[14].addPoint([x, Number(obj.batt4data3)], true, true, true);
    } else {
      chartT.series[14].addPoint([x, Number(obj.batt4data3)], true, false, true);
    }
	if(chartT.series[15].data.length > MAXCHARTDATA) {
      chartT.series[15].addPoint([x, Number(obj.batt4data4)], true, true, true);
    } else {
      chartT.series[15].addPoint([x, Number(obj.batt4data4)], true, false, true);
    }
	if(chartT.series[16].data.length > MAXCHARTDATA) {
      chartT.series[16].addPoint([x, Number(obj.batt5data1)], true, true, true);
    } else {
      chartT.series[16].addPoint([x, Number(obj.batt5data1)], true, false, true);
    }
	if(chartT.series[17].data.length > MAXCHARTDATA) {
      chartT.series[17].addPoint([x, Number(obj.batt5data2)], true, true, true);
    } else {
      chartT.series[17].addPoint([x, Number(obj.batt5data2)], true, false, true);
    }
	if(chartT.series[18].data.length > MAXCHARTDATA) {
      chartT.series[18].addPoint([x, Number(obj.batt5data3)], true, true, true);
    } else {
      chartT.series[18].addPoint([x, Number(obj.batt5data3)], true, false, true);
    }
	if(chartT.series[19].data.length > MAXCHARTDATA) {
      chartT.series[19].addPoint([x, Number(obj.batt5data4)], true, true, true);
    } else {
      chartT.series[19].addPoint([x, Number(obj.batt5data4)], true, false, true);
    }
	if(chartT.series[20].data.length > MAXCHARTDATA) {
      chartT.series[20].addPoint([x, Number(obj.batt6data1)], true, true, true);
    } else {
      chartT.series[20].addPoint([x, Number(obj.batt6data1)], true, false, true);
    }
	if(chartT.series[21].data.length > MAXCHARTDATA) {
      chartT.series[21].addPoint([x, Number(obj.batt6data2)], true, true, true);
    } else {
      chartT.series[21].addPoint([x, Number(obj.batt6data2)], true, false, true);
    }
	if(chartT.series[22].data.length > MAXCHARTDATA) {
      chartT.series[22].addPoint([x, Number(obj.batt6data3)], true, true, true);
    } else {
      chartT.series[22].addPoint([x, Number(obj.batt6data3)], true, false, true);
    }
	if(chartT.series[23].data.length > MAXCHARTDATA) {
      chartT.series[23].addPoint([x, Number(obj.batt6data4)], true, true, true);
    } else {
      chartT.series[23].addPoint([x, Number(obj.batt6data4)], true, false, true);
    }
	if(chartT.series[24].data.length > MAXCHARTDATA) {
      chartT.series[24].addPoint([x, Number(obj.batt7data1)], true, true, true);
    } else {
      chartT.series[24].addPoint([x, Number(obj.batt7data1)], true, false, true);
    }
	if(chartT.series[25].data.length > MAXCHARTDATA) {
      chartT.series[25].addPoint([x, Number(obj.batt7data2)], true, true, true);
    } else {
      chartT.series[25].addPoint([x, Number(obj.batt7data2)], true, false, true);
    }
	if(chartT.series[26].data.length > MAXCHARTDATA) {
      chartT.series[26].addPoint([x, Number(obj.batt7data3)], true, true, true);
    } else {
      chartT.series[26].addPoint([x, Number(obj.batt7data3)], true, false, true);
    }
	if(chartT.series[27].data.length > MAXCHARTDATA) {
      chartT.series[27].addPoint([x, Number(obj.batt7data4)], true, true, true);
    } else {
      chartT.series[27].addPoint([x, Number(obj.batt7data4)], true, false, true);
    }
	if(chartT.series[28].data.length > MAXCHARTDATA) {
      chartT.series[28].addPoint([x, Number(obj.batt8data1)], true, true, true);
    } else {
      chartT.series[28].addPoint([x, Number(obj.batt8data1)], true, false, true);
    }
	if(chartT.series[29].data.length > MAXCHARTDATA) {
      chartT.series[29].addPoint([x, Number(obj.batt8data2)], true, true, true);
    } else {
      chartT.series[29].addPoint([x, Number(obj.batt8data2)], true, false, true);
    }
	if(chartT.series[30].data.length > MAXCHARTDATA) {
      chartT.series[30].addPoint([x, Number(obj.batt8data3)], true, true, true);
    } else {
      chartT.series[30].addPoint([x, Number(obj.batt8data3)], true, false, true);
    }
	if(chartT.series[31].data.length > MAXCHARTDATA) {
      chartT.series[31].addPoint([x, Number(obj.batt8data4)], true, true, true);
    } else {
      chartT.series[31].addPoint([x, Number(obj.batt8data4)], true, false, true);
    }
	
  }, false);
}