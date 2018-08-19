window.onload = function () {
    setInterval(function () {
        fnTime();
    },1000);

}


function fnTime() {
    var timeDisplay = document.getElementById('time-display');
    var date = new Date();
    var hours = date.getHours();
    var minute = date.getMinutes();
    var second = date.getSeconds();
    var time = fnW(hours)+":"+fnW(minute)+":"+fnW(second);
    timeDisplay.innerHTML = time;
}
//补位 当某个字段不是两位数时补0
function fnW(str){
var num;
str>=10?num=str:num="0"+str;
return num;
}