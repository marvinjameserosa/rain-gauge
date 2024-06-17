function updateSensorValues() {
  fetch('/sensor_data')
      .then(response => response.json())
      .then(data => {
          var rpm = data.rpm;
          document.getElementById("rpm-val").textContent = rpm;
          var flowrate = data.flowrate;
          document.getElementById("flowrate-val").textContent = flowrate;
          var mm = data.mm;
          document.getElementById("mm-val").textContent = mm;
          var intensity = data.intensity;
          document.getElementById("intensity-val").textContent = intensity;
      });
}

function resetData() {
    fetch('/reset_data', { method: 'POST' })
        .then(response => response.json())
        .then(data => {
            console.log(data.message);
            // Optionally update UI elements on successful reset
            document.getElementById("rpm-val").textContent = '0.00 rpm';
            document.getElementById("flowrate-val").textContent = '0.00 m3/s';
            document.getElementById("mm-val").textContent = '0.00';
            document.getElementById("intensity-val").textContent = 'No Rain';
        })
        .catch(error => {
            console.error('Error resetting data:', error);
            alert('Error resetting data');
        });
}

setInterval(updateSensorValues, 100);
