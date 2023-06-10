//import "./handlebars.js";
import './helpers.js';

var screens = ["Block Height", "Moscow Time", "Ticker", "Time", "Halving countdown"];

getBcStatus = () => {
    fetch('http://btclock3.local/api/status', {
        method: 'get'
    })
        .then(response => response.json())
        .then(jsonData => {
            var source = document.getElementById("entry-template").innerHTML;
            var template = Handlebars.compile(source);
            var context = { currentScreen: jsonData.currentScreen, rendered: jsonData.rendered, data: jsonData.data, screens: screens, ledStatus: jsonData.ledStatus ? jsonData.ledStatus.map((t) => (t).toString(16)) : [] };
            document.getElementById('output').innerHTML = template(context);
        })
        .catch(err => {
            //error block
        });
}

interval = setInterval(getBcStatus, 2500);
getBcStatus();

fetch('http://btclock3.local/api/settings', {
    method: 'get'
})
    .then(response => response.json())
    .then(jsonData => {
        var fgColor = ("0x" + jsonData.fgColor.toString(16).toUpperCase());
        if (jsonData.epdColors == 2) {
            document.getElementById('fgColor').querySelector('[value="0xF800"]').remove();
            document.getElementById('bgColor').querySelector('[value="0xF800"]').remove();
        }

        document.getElementById('output').classList.add("fg-" + jsonData.fgColor.toString(16));
        document.getElementById('output').classList.add("bg-" + jsonData.bgColor.toString(16));

        document.getElementById('fgColor').value = fgColor;
        document.getElementById('bgColor').value = "0x" + jsonData.bgColor.toString(16).toUpperCase();

        if (jsonData.ledFlashOnUpdate)
            document.getElementById('ledFlashOnUpdate').checked = true;

        document.getElementById('timePerScreen').value = jsonData.timerSeconds / 60;

        var source = document.getElementById("screens-template").innerHTML;
        var template = Handlebars.compile(source);
        var context = { screens: jsonData.screens };
        document.getElementById('outputScreens').innerHTML = template(context);
    })
    .catch(err => {
        console.log('error', err);
    });


var settingsForm = document.querySelector('#settingsForm');
settingsForm.onsubmit = (event) => {
    var formData = new FormData(settingsForm);

    fetch("http://btclock3.local/api/settings",
        {
            body: formData,
            method: "post"
        }).then(() => {
            console.log('Submitted');
        });

    return false;
}


var ledsForm = document.querySelector('#ledsForm');
ledsForm.onsubmit = (event) => {
    var formData = new FormData(ledsForm);

    fetch('/api/lights/' + encodeURIComponent(formData.get('pickedColor').substring(1)), {
        method: 'get'
    })

    return false;
}

turnOffLedsBtn.onclick = (event) => {
    fetch('/api/lights/off', {
        method: 'get'
    })
    return false;
}

var textForm = document.querySelector('#customTextForm');
textForm.onsubmit = (event) => {
    var formData = new FormData(textForm);

    fetch('/api/show/text/' + encodeURIComponent(formData.get('customText')), {
        method: 'get'
    })
        .then(response => response.json())
        .catch(err => {
            //error block
        });
    return false;
}

changeScreen = (id) => {
    fetch('/api/show/screen/' + encodeURIComponent(id), {
        method: 'get'
    })
        .then(response => response.json())
        .catch(err => {
            //error block
        });
}