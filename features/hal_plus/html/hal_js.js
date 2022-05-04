function setTable(what){
	if(document.getElementById(what).style.display=="none"){
		document.getElementById(what).style.display="table";
	}else if(document.getElementById(what).style.display=="table"){
		document.getElementById(what).style.display="none";
	}
}
// get get variable
function getVar(){
	var qs = (function(a) {
		if (a == "") return {};
		var b = {};
		for (var i = 0; i < a.length; ++i)
		{
		    var p=a[i].split('=', 2);
		    if (p.length == 1)
		        b[p[0]] = "";
		    else
		        b[p[0]] = decodeURIComponent(p[1].replace(/\+/g, " "));
		}
		return b;
	})(window.location.search.substr(1).split('&'));
	return qs;
}
// color table with active task
function checkTask(){
	var getters = getVar();
	if(getters["task_id"]){
		var task_table = 'task_table_'+getters["task_id"];
		var Doc = document.getElementById(task_table);
		Doc.style.display = "table";
		Doc.style.backgroundColor="#006500";
	}
}

function changeImage(obj_name, names) {
	var pics = document.getElementById(obj_name);
	var point = 0;
	for(i =0;i<names.length;i++){
		if(pics.getAttribute('src').valueOf()==names[i].valueOf()){ 
			point = i;
		}
	}
	point = (point+1)%names.length;
	pics.src = names[point];
}
function setRow(what){
	var elements = document.getElementsByClassName(what);
	var temp;
	for(var i=0; i<elements.length; i++) {
		temp=elements[i];
		if(temp.style.display=="none")
			temp.style.display='';
		else
			temp.style.display="none";
}}
