
Highcharts.chart('container', {

    title: {
      text: 'Population Analysis by Generation'
    },
  
    subtitle: {
      text: 'Instance: weighted_500.txt'
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
        csvURL: './max_mean.csv',
        enablePolling: true,
        dataRefreshRate: 1
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