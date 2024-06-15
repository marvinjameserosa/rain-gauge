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

setInterval(updateSensorValues, 100);
