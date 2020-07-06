//import java.util.UUID;
let server_buffer=1024*3;//Expected buffer lenght on server side
let fs = false; //full screen status
let drawPoins=true; //DEBUG registered points
let expState=0; //Experiment phase
let lst = [];   //Lines
let resultsToSend=false;
let unique="";//UUID.randomUUID().toString();- JAVA :-(

function createUUID() 
{
  return "mouse-"+(new Date()).getTime().toString(16)+Math.floor(1E7*Math.random()).toString(16);
}

function remember(X,Y,Msg){
    if(resultsToSend){ return;} // Experiment finished. Data is about to send
    if(Msg==null) {Msg="";}// some_variable == null
    let time=""+nf(hour(),2)+'.'+nf(minute(),2)+'.'+nf(second(),2)+'.'+millis();
    lst.push(time+","+X+","+Y+","+nf(X/width*100,2,2)+"%,"+nf(Y/height*100,2,2)+"%,"+Msg);
}

function problem(){
  alert("Huston! We have a problem!");
  resultsToSend=true;//AGAIN!
  loop();
}

function OK(){
  alert("Transmission completed!");
}

function sendResults(){ // See https://p5js.org/reference/#/p5/httpPost 
      if(resultsToSend)
      {
        noLoop();
        resultsToSend=false;
        unique=createUUID();
       
        let str="Wait sometime for sending data!(uuid:"+unique+")\n";
        //str+=lst[0]+"\n"+lst[1]+"\n"+lst[2]+"\n"+lst[3]+"\n"+lst[4]+"\n"+lst[5]+"\n"+
        //     lst[6]+"\n"+lst[7]+"\n"+lst[8]+"\n"+lst[9]+"\n"+lst[0xA]+"\n"+lst[0xB]+"...\n";
        
        let data="#TEST FOR MOUSE EXPERIMENT FINISHED @ "+nf(year(),4)+"-"+nf(month(),2)+"-"+nf(day(),2)+
                "T"+nf(hour(),2)+'.'+nf(minute(),2)+'.'+nf(second(),2)+'.'+millis()+"\n";
        for(var s of lst)
        {
          data+=s+"\n";
        }
        
        //All in one send. Server buffers may be overloaded!!!
        if(data.length<server_buffer)//All in one send. Server buffers may be overloaded!!!
        {
          httpPost("/"+unique+"!","text",data,OK,problem);//Send data back to server
          alert(str+"("+data.length+")");
        }
        else
        {
           alert("BIT DATA NOT IMPLEMENTED YET! ("+data.length+")");  
        }
      }
}

function setup() {      
   createCanvas(windowWidth, windowHeight);//NOTE: Even full screens may have diferent aspect ratio!!!
   background(128);
}

function windowResized() { // Definition of windowResized handler 
    resizeCanvas(windowWidth, windowHeight); 
    remember(width,height,"windowResized");
} 

function mouseMoved(){ // Definition of mouseMoved handler
  if(expState==1){
    if(drawPoins){ point(mouseX,mouseY);}
    remember(mouseX,mouseY,"MM");
  }
}

function keyPressed() { // Definition of keyPressed handler
    if(expState==0 && keyCode==ENTER){
      fs = fullscreen(); // Previous screen state
      fullscreen(!fs);   // Call to fullscreen function for opposite state 
      remember(width,height,"ENTER0");
      expState=1;        // Experiment started
    }
    else
    if(expState==1 && keyCode==ENTER){
      expState=2;         // Experiment finished
      remember(width,height,"ENTER1");// last line
      fs = fullscreen(); // Previous screen state
      fullscreen(!fs);  // Call to fullscreen function for opposite state
      resultsToSend=true;
    }
} 

function draw1() { // Starting panel
    background(0, 200, 0); 
    stroke(100);
    fill(0);
    textSize(30); 
    textAlign(CENTER); 
    text("FAU-Mouse\n\nPress ENTER to START and when you finish",  
         windowWidth / 2,  
         windowHeight / 2); 
} 

function draw2() { // Experiment panel
  //stroke(255,0,0);
  //line(width,0,0,height);
  stroke(0,255,255);
  //line(0,0,width,height);
  fill(0,0,255);
  ellipse(width/2,height/2,height/10,height/10);
  stroke(0);
}

function draw3() { // Final panel
    background(200,0, 0); 
    stroke(100,0,100);
    fill(0,0,100);
    textSize(30); 
    textAlign(CENTER); 
    text("FAU-Mouse\nEXPERIMENT FINISHED"+(fullscreen()?"\nPress ESCAPE!":""),  
         windowWidth / 2,  
         windowHeight / 2); 
}

function draw(){ // selection from panels
    if(expState==0){
        draw1();
    }
    else
    if(expState==1){
        draw2();
    }
    else {
        draw3();
        sendResults();//Until success - but this is blocking!
    }
}
