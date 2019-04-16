#ifndef __HTML_H__
#define __HTML_H__



static const char PROGMEM EDITOR_HTML[] = R"rawliteral(
<!DOCTYPE html>
<head>
<title>MiniPlan Motion Editor</title>
<style type=\"text/css\">
body {
  color: white;
  background-color: #000000 }
  .pm_btn {
  width: 160px;
  -webkit-border-radius: 5;
  -moz-border-radius: 5;
  border-radius: 5px;
  font-family: Arial;
  color: #ffffff;
  font-size: 24px;
  background: #3498db;
  padding: 10px 20px 10px 20px;
  text-decoration: none;
}
.pm_text {
width: 160px;
-webkit-border-radius: 5;
-moz-border-radius: 5;
border-radius: 5px;
font-family: Arial;
font-size: 24px;

padding: 10px 20px 10px 20px;
text-decoration: none;
}

.pm_btn:hover {
  background: #3cb0fd;
  background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);
  background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);
  background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);
  background-image: -o-linear-gradient(top, #3cb0fd, #3498db);
  background-image: linear-gradient(to bottom, #3cb0fd, #3498db);
  text-decoration: none;
}

.pms_btn {
width: 240px;
-webkit-border-radius: 5;
-moz-border-radius: 5;
border-radius: 5px;
font-family: Arial;
color: #ffffff;
font-size: 24px;
background: #3498db;
padding: 10px 20px 10px 20px;
text-decoration: none;
}

.pms_btn:hover {
background: #3cb0fd;
background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);
background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);
background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);
background-image: -o-linear-gradient(top, #3cb0fd, #3498db);
background-image: linear-gradient(to bottom, #3cb0fd, #3498db);
text-decoration: none;
}
</style>
</head>
<body>

<table>
<tr>
<td>GPIO 16  ,  Default value = 90<br/><input class=\"pm_text\" type=\"text\" id=\"gpid_16\" value=\"90\"><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(16, 'gpid_16')\">SEND</button></td>
<td>GPIO 14  ,  Default value = 90<br/><input class=\"pm_text\" type=\"text\" id=\"gpid_14\" value=\"90\"><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(14, 'gpid_14')\">SEND</button></td>
</tr>

<tr>
<td>GPIO 13  ,  Default value = 90<br/><input class=\"pm_text\" type=\"text\" id=\"gpid_13\" value=\"90\"><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(13, 'gpid_13')\">SEND</button></td>
<td>GPIO 12  ,  Default value = 90<br/><input class=\"pm_text\" type=\"text\" id=\"gpid_12\" value=\"90\"><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(12, 'gpid_12')\">SEND</button></td>
</tr>
</table>

<br>

<table>
<tr>
<td>Servo 8  ,  Default value = 80<br/><input class=\"pm_text\" type=\"text\" id=\"servo_8\" value=\"80\"><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(8,'servo_8')\">SEND</button></td>
<td>Servo 7  ,  Default value = 80<br/><input class=\"pm_text\" type=\"text\" id=\"servo_7\" value=\"80\"><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(7, 'servo_7')\">SEND</button></td>
</tr>

<tr>
<td>Servo 9  ,  Default value = 110<br/><input class=\"pm_text\" type=\"text\" id=\"servo_9\" value=\"110\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(9,'servo_9')\">SEND</button></td>
<td>Servo 6  ,  Default value = 50<br/><input class=\"pm_text\" type=\"text\" id=\"servo_6\" value=\"50\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(6,'servo_6')\">SEND</button></td>
</tr>

<tr>
<td>Servo 10  ,  Default value = 25<br/><input class=\"pm_text\" type=\"text\" id=\"servo_10\" value=\"25\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(10,'servo_10')\">SEND</button></td>
<td>Servo 5  ,  Default value = 135<br/><input class=\"pm_text\" type=\"text\" id=\"servo_5\" value=\"135\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(5,'servo_5')\">SEND</button></td>
</tr>
</table>

<br>

<table>
<tr>
<td>Servo 11  ,  Default value = 110<br/><input class=\"pm_text\" type=\"text\" id=\"servo_11\" value=\"110\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(11,'servo_11')\">SEND</button></td>
<td>Servo 4  ,  Default value = 50<br/><input class=\"pm_text\" type=\"text\" id=\"servo_4\" value=\"50\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(4,'servo_4')\">SEND</button></td>
</tr>

<tr>
<td>Servo 12  ,  Default value = 50<br/><input class=\"pm_text\" type=\"text\" id=\"servo_12\" value=\"50\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(12,'servo_12')\">SEND</button></td>
<td>Servo 3  ,  Default value = 110<br/><input class=\"pm_text\" type=\"text\" id=\"servo_3\" value=\"110\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(3,'servo_3')\">SEND</button></td>
</tr>

<tr>
<td>Servo 13  ,  Default value = 110<br/><input class=\"pm_text\" type=\"text\" id=\"servo_13\" value=\"110\"><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(13,'servo_13')\">SEND</button></td>
<td>Servo 2  ,  Default value = 50<br/><input class=\"pm_text\" type=\"text\" id=\"servo_2\" value=\"50\"><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(2,'servo_2')\">SEND</button></td>
</tr>

<tr>
<td>Servo 14  ,  Default value = 80<br/><input class=\"pm_text\" type=\"text\"  id=\"servo_14\" value=\"80\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(14,'servo_14')\">SEND</button></td>
<td>Servo 1  ,  Default value = 80<br/><input class=\"pm_text\" type=\"text\" id=\"servo_1\" value=\"80\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(1,'servo_1')\">SEND</button></td>
</tr>

<tr>
<td>Servo 15  ,  Default value = 100<br/><input class=\"pm_text\" type=\"text\"  id=\"servo_15\" value=\"100\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(15,'servo_15')\">SEND</button></td>
<td>Servo 0  ,  Default value = 60<br/><input class=\"pm_text\" type=\"text\" id=\"servo_0\" value=\"60\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(0,'servo_0')\">SEND</button></td>
</tr>
</table>

<br>

<table>
<tr>
<td><button class=\"pm_btn\" style=\"background: #ed3db5;\" type=\"button\" onclick=\"controlPm(99)\">STANDBY POSE</button></td>
</tr>
</table>

<br>

</body>
<script>

function controlServo(id, textId) {
  var xhttp = new XMLHttpRequest();
  var value = document.getElementById(textId).value;
  xhttp.onreadystatechange = function() {
    if (xhttp.readyState == 4 && xhttp.status == 200) {
     document.getElementById(\"demo\").innerHTML = xhttp.responseText;
    }
  };
  xhttp.open(\"GET\",\"controller?servo=\"+id+\"&value=\"+value, true);
  xhttp.send();
}
function controlGpid(id, textId) {
  var xhttp = new XMLHttpRequest();
  var value = document.getElementById(textId).value;
  xhttp.onreadystatechange = function() {
    if (xhttp.readyState == 4 && xhttp.status == 200) {
     document.getElementById(\"demo\").innerHTML = xhttp.responseText;
    }
  };
  xhttp.open(\"GET\", \"controller?gpid=\"+id+\"&value=\"+value, true);
  xhttp.send();
}

function controlPm(value) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (xhttp.readyState == 4 && xhttp.status == 200) {
     document.getElementById(\"demo\").innerHTML = xhttp.responseText;
    }
  };
  xhttp.open(\"GET\", \"controller?pm=\"+value, true);
  xhttp.send();
}

</script>
</html>
)rawliteral";

static const char PROGMEM ZERO_HTML[] = R"rawliteral(
<!DOCTYPE html>
  <head>
  <title>MiniPlan Zero Check</title>
   <style type=\"text/css\">
    body {
      color: white;
      background-color: #000000 }
    .pm_btn {
    width: 160px;
    -webkit-border-radius: 5;
    -moz-border-radius: 5;
    border-radius: 5px;
    font-family: Arial;
    color: #ffffff;
    font-size: 24px;
    background: #3498db;
    padding: 10px 20px 10px 20px;
    text-decoration: none;
  }
  .pm_text {
  width: 160px;
  -webkit-border-radius: 5;
  -moz-border-radius: 5;
  border-radius: 5px;
  font-family: Arial;
  font-size: 24px;
  padding: 10px 20px 10px 20px;
  text-decoration: none;
  }
  .pm_btn:hover {
    background: #3cb0fd;
    background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);
    background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);
    background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);
    background-image: -o-linear-gradient(top, #3cb0fd, #3498db);
    background-image: linear-gradient(to bottom, #3cb0fd, #3498db);
    text-decoration: none;
  }
  .pms_btn {
  width: 240px;
  -webkit-border-radius: 5;
  -moz-border-radius: 5;
  border-radius: 5px;
  font-family: Arial;
  color: #ffffff;
  font-size: 24px;
  background: #3498db;
  padding: 10px 20px 10px 20px;
  text-decoration: none;
  }
  .pms_btn:hover {
  background: #3cb0fd;
  background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);
  background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);
  background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);
  background-image: -o-linear-gradient(top, #3cb0fd, #3498db);
  background-image: linear-gradient(to bottom, #3cb0fd, #3498db);
  text-decoration: none;
  }
  </style>
  </head>
  <body>
  <table>
  <tr>
  <td><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(16, 90)\">GPIO 16</button></td>
  <td><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(14, 90)\">GPIO 14</button></td>
  </tr>
  <tr>
  <td><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(13, 90)\">GPIO 13</button></td>
  <td><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(12, 90)\">GPIO 12</button></td>
  </tr>
  </table>
  <br>
  <table>
  <tr>
  <td><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(8,80)\">Servo 8</button></td>
  <td><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(7,80)\">Servo 7</button></td>
  </tr>
  <tr>
  <td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(9,110)\">Servo 9</button></td>
  <td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(6,50)\">Servo 6</button></td>
  </tr>
  <tr>
  <td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(10,25)\">Servo 10</button></td>
  <td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(5,135)\">Servo 5</button></td>
  </tr>
  </table>
  <br>
  <table>
  <tr>
  <td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(11,110)\">Servo 11</button></td>
  <td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(4,50)\">Servo 4</button></td>
  </tr>
  <tr>
  <td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(12,50)\">Servo 12</button></td>
  <td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(3,110)\">Servo 3</button></td>
  </tr>
  <tr>
  <td><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(13,110)\">Servo 13</button></td>
  <td><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(2,50)\">Servo 2</button></td>
  </tr>
  <tr>
  <td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(14,80)\">Servo 14</button></td>
  <td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(1,80)\">Servo 1</button></td>
  </tr>
  <tr>
  <td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(15,100)\">Servo 15</button></td>
  <td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(0,60)\">Servo 0</button></td>
  </tr>
  </table>
  <br>
  <table>
  <tr>
  <td><button class=\"pm_btn\" style=\"background: #ed3db5;\" type=\"button\" onclick=\"controlPm(100)\">ALL</button></td>
  </tr>
  </table>
  <br>
  </body>
  <script>
  function controlServo(id, value) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (xhttp.readyState == 4 && xhttp.status == 200) {
       document.getElementById(\"demo\").innerHTML = xhttp.responseText;
      }
    };
    xhttp.open(\"GET\", \"controller?servo=\"+id+\"&value=\"+value, true);
    xhttp.send();
  }
  function controlGpid(id, value) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (xhttp.readyState == 4 && xhttp.status == 200) {
       document.getElementById(\"demo\").innerHTML = xhttp.responseText;
      }
    };
    xhttp.open(\"GET\", \"controller?gpid=\"+id+\"&value=\"+value, true);
    xhttp.send();
  }
  function controlPm(value) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (xhttp.readyState == 4 && xhttp.status == 200) {
       document.getElementById(\"demo\").innerHTML = xhttp.responseText;
      }
    };
    xhttp.open(\"GET\", \"controller?pm=\"+value, true);
    xhttp.send();
  }
  </script>
  </html>
)rawliteral";

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<html>
  <head>
    <title>MiniPlan Controller</title>
    <style type=\"text/css\">
    body {
      color: white;
      background-color: #000000 
	}
    .pm_btn {
	  width: 160px;
      -webkit-border-radius: 5;
      -moz-border-radius: 5;
      border-radius: 5px;
      font-family: Arial;
      color: #ffffff;
      font-size: 24px;
      background: #3498db;
      padding: 10px 20px 10px 20px;
      text-decoration: none;
    }
    .pm_btn:hover {
      background: #3cb0fd;
      background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);
      background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);
      background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);
      background-image: -o-linear-gradient(top, #3cb0fd, #3498db);
      background-image: linear-gradient(to bottom, #3cb0fd, #3498db);
      text-decoration: none;
    }
    .pms_btn {
      width: 240px;
      -webkit-border-radius: 5;
      -moz-border-radius: 5;
      border-radius: 5px;
      font-family: Arial;
      color: #ffffff;
      font-size: 24px;
      background: #3498db;
      padding: 10px 20px 10px 20px;
      text-decoration: none;
    }
    .pms_btn:hover {
      background: #3cb0fd;
      background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);
      background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);
      background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);
      background-image: -o-linear-gradient(top, #3cb0fd, #3498db);
      background-image: linear-gradient(to bottom, #3cb0fd, #3498db);
      text-decoration: none;
    }
    </style>
  </head>
  <body>
  <table>
  <tr>
  <td><button  class=\"pm_btn\" type=\"button\" onclick=\"controlPm(3)\">TurnLeft</button></td>
  <td><button  class=\"pm_btn\" type=\"button\" onclick=\"controlPm(1)\">Forward</button></td>
  <td><button  class=\"pm_btn\" type=\"button\" onclick=\"controlPm(4)\">TurnRight</button></td>
  </tr>
  <tr>
  <td><button  class=\"pm_btn\" type=\"button\" onclick=\"controlPm(5)\">MoveLeft</button></td>
  <td><button  class=\"pm_btn\" style=\"background: #ed3db5;\" type=\"button\" onclick=\"controlPm(99)\">STANDBY</button></td>
  <td><button  class=\"pm_btn\" type=\"button\" onclick=\"controlPm(6)\">MoveRight</button></td>
  </tr>
  <tr>
  <td><button class=\"pm_btn\" type=\"button\" onclick=\"controlPm(7)\">LeftKick</button></td>
  <td><button  class=\"pm_btn\" type=\"button\" onclick=\"controlPm(2)\">Backward</button></td>
  <td><button class=\"pm_btn\" type=\"button\" onclick=\"controlPm(8)\">RightKick</button></td>
  </tr>
  </table>
  <table>
  <tr>
  <td><button class=\"pms_btn\" type=\"button\" onclick=\"controlPm(9)\">SkatingForward</button></td>
  <td><button class=\"pms_btn\" type=\"button\" onclick=\"controlPm(10)\">SkatingBackward</button></td>
  </tr>
  <tr>
  <td><button class=\"pms_btn\" type=\"button\" onclick=\"controlPm(11)\">GetUp</button></td>
  <td><button class=\"pms_btn\" type=\"button\" onclick=\"controlPm(12)\">FaceDownGetUp</button></td>
  </tr>
  </table>
  <table>
  <tr>
  <td><button class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(1)\">Bow</button></td>
  <td><button  class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(2)\">Waving</button></td>
  </tr>
  <tr>
  <td><button class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(7)\">Clap Hands</button></td>
  <td><button  class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(3)\">Iron Man</button></td>
  </tr>
  <tr>
  <td><button class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(4)\">Apache</button></td>
  <td><button  class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(5)\">Balance</button></td>
  </tr>
  <tr>
  <td><button class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(6)\">Warm-Up</button></td>
  <td><button  class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(8)\">Dance</button></td>
  </tr>
  <tr>
  <td><button class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(9)\">Push-Up</button></td>
  <td><button  class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(10)\">TaiChi</button></td>
  </tr>
  <tr>
  <td colspan=\"2\"><center><button  class=\"pms_btn\" style=\"background: #04b404;\" type=\"button\" onclick=\"controlPms(99)\">Auto Demo</button></center></td>
  </tr>
  </table>
  </body>
  <script>
  function controlPm(id) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (xhttp.readyState == 4 && xhttp.status == 200) {
      }
    };
    xhttp.open('GET', 'controller?pm='+id, true);
    xhttp.send();
  }
  function controlPms(id) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (xhttp.readyState == 4 && xhttp.status == 200) {
      }
    };
    xhttp.open('GET', 'controller?pms='+id, true);
    xhttp.send();
  }
  </script>
  </html>
)rawliteral";



  
#endif
