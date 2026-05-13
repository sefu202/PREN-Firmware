from flask import Flask, jsonify, render_template_string, request
import Core.Src.Comm.ControlUnit as ControlUnit
app = Flask(__name__)

HTML = """
<!DOCTYPE html>
<html>
<head>
    <title>ControlUnit Monitor</title>

    <style>
        body {
            font-family: Arial, sans-serif;
            background: #1e1e1e;
            color: white;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }

        .panel {
            background: #2b2b2b;
            padding: 30px;
            border-radius: 16px;
            width: 500px;
            box-shadow: 0 0 20px rgba(0,0,0,0.5);
        }

        h1 {
            text-align: center;
        }

        .row {
            display: flex;
            justify-content: space-between;
            margin: 12px 0;
            padding: 10px;
            background: #3a3a3a;
            border-radius: 8px;
        }

        .label {
            font-weight: bold;
        }
    </style>
</head>
<body>

<div class="panel">
    <h1>Control Unit live view</h1>

    <div class="row">
        <div class="label">X Position</div>
        <div id="x">0</div>
    </div>

    <div class="row">
        <div class="label">Y Position</div>
        <div id="y">0</div>
    </div>

    <div class="row">
        <div class="label">Z Position</div>
        <div id="z">0</div>
    </div>

    <div class="row">
        <div class="label">Rotation</div>
        <div id="rot">0</div>
    </div>

    <div class="row">
        <div class="label">Vacuum</div>
        <div id="vacuum">OFF</div>
    </div>

    <div class="row">
        <div class="label">E-Stop</div>
        <div id="estop">FALSE</div>
    </div>

    <div class="row">
        <div class="label">Start Button</div>
        <div id="start">FALSE</div>
    </div>

    <div class="row">
        <div class="label">Initialized</div>
        <div id="init">FALSE</div>
    </div>

    <hr style="margin:20px 0; border-color:#555;">

    <h2>Controls</h2>

    <div class="row">
        <input id="xTarget" type="number" placeholder="X Target">
        <button onclick="setX()">Set X</button>
    </div>

    <div class="row">
        <input id="yTarget" type="number" placeholder="Y Target">
        <button onclick="setY()">Set Y</button>
    </div>

    <div class="row">
        <input id="zTarget" type="number" placeholder="Z Target">
        <button onclick="setZ()">Set Z</button>
    </div>

    <div class="row">
        <input id="rotTarget" type="number" placeholder="Rotation">
        <button onclick="setRot()">Set Rotation</button>
    </div>

    <div class="row">
        <button onclick="vacuum(true)">Vacuum ON</button>
        <button onclick="vacuum(false)">Vacuum OFF</button>
    </div>

    <div class="row">
        <input id="r" type="number" min="0" max="255" placeholder="R">
        <input id="g" type="number" min="0" max="255" placeholder="G">
        <input id="b" type="number" min="0" max="255" placeholder="B">
        <button onclick="setLed()">Set LED</button>
    </div>

    <div class="row">
        <button onclick="shootConfetti()">CONFETTI</button>
    </div>
</div>
</div>

<script>
async function updateData() {
    const response = await fetch('/process-image');
    const data = await response.json();

    document.getElementById('x').innerText = data.x;
    document.getElementById('y').innerText = data.y;
    document.getElementById('z').innerText = data.z;
    document.getElementById('rot').innerText = data.rotation;
    document.getElementById('vacuum').innerText = data.vacuum;
    document.getElementById('estop').innerText = data.estop;
    document.getElementById('start').innerText = data.start;
    document.getElementById('init').innerText = data.initialized;
}

async function setX() {
    const value = document.getElementById('xTarget').value;
    await fetch(`/set-x/${value}`);
}

async function setY() {
    const value = document.getElementById('yTarget').value;
    await fetch(`/set-y/${value}`);
}

async function setZ() {
    const value = document.getElementById('zTarget').value;
    await fetch(`/set-z/${value}`);
}

async function setRot() {
    const value = document.getElementById('rotTarget').value;
    await fetch(`/set-rot?value=${value}`);
}

async function vacuum(state) {
    await fetch(`/vacuum/${state}`);
}

async function setLed() {
    const r = document.getElementById('r').value;
    const g = document.getElementById('g').value;
    const b = document.getElementById('b').value;

    await fetch(`/set-led/${r}/${g}/${b}`);
}

async function shootConfetti() {
    await fetch('/confetti');
}

setInterval(updateData, 1000);
updateData();
</script>

</body>
</html>
"""


@app.route('/')
def index():
    return render_template_string(HTML)


@app.route('/process-image')
def process_image():
    pi = cu.getProcessImage()

    return jsonify({
        'x': pi.xPositionSteps,
        'y': pi.yPositionSteps,
        'z': pi.zPositionSteps,
        'rotation': pi.rotationSteps,
        'vacuum': pi.vacuumOn,
        'estop': pi.btnEStop,
        'start': pi.btnStart,
        'initialized': pi.isInitialized
    })


@app.route('/set-x/<int:value>')
def set_x(value):
    cu.setXTarget(value)
    return 'OK'


@app.route('/set-y/<int:value>')
def set_y(value):
    cu.setYTarget(value)
    return 'OK'


@app.route('/set-z/<int:value>')
def set_z(value):
    cu.setZTarget(value)
    return 'OK'


@app.route('/set-rot')
def set_rot():
    value = int(request.args.get('value'))
    cu.setRot(value)
    return 'OK'


@app.route('/vacuum/<state>')
def set_vacuum(state):
    cu.setEnableVacuum(state.lower() == 'true')
    return 'OK'


@app.route('/set-led/<int:r>/<int:g>/<int:b>')
def set_led(r, g, b):
    cu.setLed(r, g, b)
    return 'OK'


@app.route('/confetti')
def confetti():
    cu.shootConfetti()
    return 'OK'

if __name__ == "__main__":

    cu = ControlUnit.ControlUnit()
    cu.startCommunication()

    app.run(host="0.0.0.0", port=5000, debug=True)