var ball_initial_x = 470;//球初始坐标left  
var ball_initial_y = 555;//球初始坐标top  
var angle = 89;//初始飞行角度  
var zx = 1;//控制left位移的正负  
var zy = -1;//控制top位移的正负
var rp = null;//控制游戏进程  
      
//var qx1=0;  
//var ball_initial_y1=0;  
      
      
function go(){
	"use strict";
	var ball = $("ball");  
	
	ball_initial_x = ball_initial_x + zx*Math.cos((2*Math.PI/360)*angle);  
	ball_initial_y = ball_initial_y + zy*Math.sin((2*Math.PI/360)*angle);  
              
	if(ball_initial_y>=550){  
		if(ball_initial_x<board_left||ball_initial_x>board_left+100){//判断是否接住  
			clearTimeout(rp); 
		}else{  
			zy=-1;  
			if((ball_initial_x-board_left)>(75)){  
				angle = 90-(ball_initial_x-board_left+10-75)/75*90;  
				zx = 1;  
			}else{
				angle = 90 - (75-(ball_initial_x-board_left+10))/75*90;  
				zx=-1;  
			}  
			rp = setTimeout("go()",1);  
		}  
	}else if(ball_initial_y<=480){
		for(var i=0,m = bricks.length ;i<m;i++){  
			var io = checkIsP(ball_initial_x,ball_initial_y,bricks[i].offsetLeft,bricks[i].offsetTop);  
                    
			if(io!==0){  
				bricks[i].style.display = "none";  
                bricks.splice(i,1); 
				if(io===1){  
					zx=1;  
				}  
                if(io===2){
					zx=-1;  
                }  
                if(io===3){  
					zy=1;  
                }  
				if(io===4){  
                    zy=-1;  
                }  
				break;  
			}                 
		}  
                  
		  if(ball_initial_x>=600){
			  zx=-1;  
		  }
		  if(ball_initial_x<=0){
			  zx=1;
		  }  
		  if(ball_initial_y<=0){
			  zy=1;
		  }	   
		  ball.style.left = ball_initial_x+"px";     
		  ball.style.top = ball_initial_y+"px";  
		  //如果砖块未打完
		  if(bricks.length!==0){  
			  rp = setTimeout("go()",1);
		  }
	}
}

function checkIsP(ball_initial_x,ball_initial_y,zx,zy){
	"use strict";
        var f = {  
            x:ball_initial_x,  
            y:ball_initial_y,  
            x1:ball_initial_x+10,  
            y1:ball_initial_y+10  
        };
        var z = {  
            x:zx,  
            y:zy,  
            x1:zx+30,  
            y1:zy+15  
        }; 
        var sx;var sy;  
        sx = f.x>=z.x?f.x:z.x;  
        sy = f.y>=z.y?f.y:z.y;  
        if(sx >= f.x && sx <= f.x1 && sy >= f.y && sy <= f.y1 && sx >= z.x && sx <= z.x1 && sy >= z.y && sy <= z.y1){  
          
            return seSmall(Math.abs(f.x-z.x1),Math.abs(f.x1-z.x),Math.abs(f.y-z.y1),Math.abs(f.y1-z.y));  
              
        }else{  
            return 0;  
        }  
    }  
      
    function seSmall(a,b,c,d){  
        "use strict";
        if(a<b&&a<c&&a<d){  
            return 1;  
        }  
        if(b<a&&b<c&&b<d){  
            return 2;  
        }  
        if(c<a&&c<b&&c<d){  
            return 3;  
        }  
        if(d<b&&d<c&&d<a){  
            return 4;  
        }  
    }  
      
    var board_left = 420;  
    var ballleft = 295;  
    var bs = 10;//棒的移动频率  
    var kflag =  false;  
    
//键盘处理事件  
function keydownEvent(event){  
	"use strict";
	if(event.keyCode===32){//如果是空格  
		if(!kflag){
			go();
			kflag = true;  
        }  
              
		var ball = $("ball");  
		var bang = $("board");  
        if(event.keyCode===37){//如果是左箭头  
			for(var i = 0;i<bs;i++){  
				if(board_left-1!==0){  
					board_left-=1;  
                    bang.style.left = board_left+"px";  
                    if(!kflag){  
						ballleft-=1;  
						ball_initial_x-=1;  
                        ball.style.left = ballleft+"px";  
                    }  
				}  
			} 
		}  
        if(event.keyCode===39){//如果是右箭头
			for(var i = 0;i<bs;i++){  
				if(board_left+1!==450){  
					board_left+=1;  
                    bang.style.left = board_left+"px";  
                    if(!kflag){  
                        ballleft+=1;  
                        ball_initial_x+=1;  
                        ball.style.left = ballleft+"px"; 
					}  
				} 
			}  
        }  
              
	}  
}