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

def reset_data():
    receiverserial.data_dict['rpm'] = '0.00 rpm'
    receiverserial.data_dict['flowrate'] = '0.00 m3/s'
    receiverserial.data_dict['mm'] = '0.00'
    receiverserial.data_dict['intensity'] = 'No Rain'
    receiverserial.data_dict['exception'] = ''

@app.route('/reset_data', methods=['POST'])
def reset_data_route():
    reset_data()
    return jsonify({'message': 'Data reset successfully'})

if __name__ == "__main__":
    threading.Thread(target=open_browser).start()
    app.run(debug=False)
