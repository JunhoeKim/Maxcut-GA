
Highcharts.chart('container', {

    title: {
      text: 'Maximum Fitness of each reset stage'
    },
  
    subtitle: {
      text: 'Instance: cubic_1000.txt'
    },
  
    yAxis: {
      title: {
        text: 'Fitness'
      }
    },

    xAxis: {
        title: {
            text: 'Generation'   
        }
    },

    legend: {
      layout: 'vertical',
      align: 'right',
      verticalAlign: 'middle'
    },
    data: {
        csv: document.getElementById('csv').innerHTML,
        csvURL: './max_mean.csv'
    },

    plotOptions: {
        series: {
            lineWidth: 1.2
        }
    },
  
    responsive: {
      rules: [{
        condition: {
          maxWidth: 500
        },
        chartOptions: {
          legend: {
            layout: 'horizontal',
            align: 'center',
            verticalAlign: 'bottom'
          }
        }
      }]
    }
  
  });