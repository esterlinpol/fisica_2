const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">
        <title>Distribucion de Fluidos</title>
        <style>html { 
                background-color: black;
                color: white;
                font-family: Helvetica; 
                display: inline-block; 
                margin: 0px auto; 
                text-align: center;
                }
                
        </style>
    </head>
    <body>
        <div id="webpage">
            <h1>Sistema de Monitoreo</h1>
            <span id="sonar1value">0</span>
        </div>
        <script>
            setInterval(function() {
            // Call a function repetatively with 2 Second interval
            getData();
            }, 2000); //2000mSeconds update rate
            
            function getData() {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                      document.getElementById("sonar1value").innerHTML =
                      this.responseText;
                      
                    }
                  };
                  xhttp.open("GET", "sonar1", true);
                  xhttp.send();
                }
        </script>    
    </body>
</html>
)=====";