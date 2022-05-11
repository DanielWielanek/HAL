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

/*
check if cell was clisked and the change state of checkbos
used by QA
*/

function chageCheckBoxCell(object){
	var check = document.getElementsByName(object)[0];
	if(check.checked==true) check.checked = false;
	else check.checked = true;
}
/*
draw the HTML page with many qa plots
*/
function qaPopup(what){

	var plots = document.getElementsByClassName("qa_"+what);
	if(plots.length==0) return;
	//var opened = window.open("");
	var plotsTxt = [];
	
	for (i = 0; i < plots.length; i++) {
	        row = plots[i];
		var Cells = row.getElementsByTagName("td");
		if(Cells.length>2){
			var cellButton = Cells[0];
			if(cellButton.childNodes[0].checked==true){
			
			var cellHisto = Cells[2];
			var ahref = cellHisto.childNodes[0];
			plotsTxt.push(ahref.getAttribute("href"));
			}
		}
	} 
	var headTxt = document.getElementsByTagName("head")[0].innerHTML;
	var content = "<html><head>"+headTxt+"</head><body>\n";
	content = content +"<script type='text/javascript'>\n";
	for(i =0; i<plotsTxt.length;i++){
		var rootPath = plotsTxt[i];
		rootPath = rootPath.replace(".html",".root");
		content = content + "\n var filename"+i+"=\"" + rootPath + "\";\n";
		content = content + "JSROOT.OpenFile(filename"+i+", function(file){\n";
		content = content + "JSROOT.gStyle.Palette= 50;\n";
		content = content + "file.ReadObject(\"canvas;1\", function(obj) {\n";
		content = content + "JSROOT.draw(\"drawing"+i+"\", obj, \"colz\");\n";
		content = content + ";});\n;});\n;";
		/*
		var filename = "hist_2d_0.root";
			 JSROOT.OpenFile(filename, function(file) {
			 JSROOT.gStyle.Palette= 50;
			file.ReadObject("canvas;1", function(obj) {
			JSROOT.draw("drawing", obj, "colz");
			;});
			;});
			*/

	}
	content = content + "</script>\n";
	var l = plotsTxt.length;
	var style="width:90%;height:900px";
	var style2="width:90%;height:900px";
	var per_row = 1;
	if(l>4){ //draw 2 per line
		style="width:45%;height:450px";
		style2="width:90%;height:450px";
		per_row = 2;
	}
	if(l>8){//draw 3 per line
		style="width:30%;height:300px";
		style2="width:90%;height:300px";
		per_row = 3;
	}
	if(l>20){//draw 4 per line
		style="width:25%;height:250px";
		style2="width:90%;height:250px";
		per_row = 4;
	}
	var closed = false;
	content = content +"<table style=\"width:100%; border: 1px solid black\" >";
	for(i =0; i<plotsTxt.length;i++){
		if(i%per_row==0){
			closed = false;
			content = content + "<tr>";
		}
		content = content +"<td id=\"drawing"+i+"\" style = \""+style+"\"></td>\n";
		if(i%per_row==per_row-1){
			content = content +"</tr>\n";
			closed = true;
		}
	}
	if(closed==false){
		content = content +"</tr>\n";
	}

	content = content +"</body>";
	
	var opened = window.open("");
	opened.document.write(content);
	
}
