from flask import Flask, render_template, jsonify
import receiverserial  
import webbrowser
import threading

app = Flask(__name__)

@app.route('/')
def index():
    return render_template("index.html")

@app.route('/sensor_data')
def get_sensor_data():
    return jsonify(receiverserial.data_dict)

def open_browser():
    webbrowser.open("http://127.0.0.1:5000")

if __name__ == "__main__":
    threading.Thread(target=open_browser).start()
    app.run(debug=False)
