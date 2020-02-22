const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Distribucion de Fluidos</title>
        <style>
            html { 
                background-color: black;
                color: white;
                font-family: Helvetica; 
                display: inline-block; 
                margin: 0px auto; 
                text-align: center;
                }
            :root {
                
                --height: 50px;    
                }  

            .cylinder {
                position: relative;
                overflow: hidden;
                margin: 0 auto;
                width: 100px;
                height: 260px;
                border-radius: 50px/25px;
                background-color: rgba(160, 160, 160, 0.89);
                }
            .cylinder:before {
                position: absolute;
                left: 0;
                top: 0;
                width: 100px;
                height: 50px;
                border-radius: 50px/25px;
                background-color: rgba(37, 33, 33, 0.2);
                content: '';
                }
            .cylinder:after {
                position: absolute;
                left: 0;
                bottom: 0;
                width: 100px;
                height: 50px;
                border-radius: 50px/25px;
                background-color: rgba(37, 33, 33, 0.2);
                content: '';
                }
            .water {
                position: absolute;
                left: 0;
                bottom: 0;
                width: 100px;
                           
                padding-top: var(--height);
                border-radius: 50px/25px;
                background-color: rgba(0, 136, 160, 0.568);
                transition: 0.3s linear;
                }    
            .water:before {
                position: absolute;
                left: 0;
                top: 0;
                width: 100px;
                height: 50px;
                border-radius: 50px/25px;
                background-color: rgba(4, 118, 138, 0.699);
                content: '';
                }
            .water:after {
                position: absolute;
                left: 0;
                bottom: 0;
                width: 100px;
                height: 50px;
                border-radius: 50px/25px;
                background-color: rgba(63, 243, 243, 0.4);
                content: '';
                }
            p {
                display:inline;
            }
            span {
                display:inline;
            }        
                  
                
        </style>
    </head>
    <body>
        <div id="webpage">
            <h1>Nivel de agua en contenedor:</h1>
            <span id="sonar1value">0</span><p>%</p>
        </div>
        <div class="cylinder">
            <div class="water"></div>
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
                      var style = getComputedStyle(document.body);
                var calculado = this.responseText * 2.5;
                          
                document.documentElement.style.setProperty('--height', calculado+'px');
                
                      
                    }
                  };
                  xhttp.open("GET", "sonar1", true);
                  
                  xhttp.send();

                }
        </script>    
    </body>
</html>
)=====";