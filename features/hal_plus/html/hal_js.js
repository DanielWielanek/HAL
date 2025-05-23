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
    var plots = document.getElementsByClassName("qa_" + what);
    if (plots.length === 0) return;

    var plotsTxt = [];
    for (let i = 0; i < plots.length; i++) {
        let row = plots[i];
        let Cells = row.getElementsByTagName("td");
        if (Cells.length > 2) {
            let cellButton = Cells[0];
            if (cellButton.childNodes[0].checked === true) {
                let cellHisto = Cells[2];
                let ahref = cellHisto.childNodes[0];
                plotsTxt.push(ahref.getAttribute("href"));
            }
        }
    }

    const l = plotsTxt.length;
    const perRow = l > 20 ? 4 : l > 8 ? 3 : l > 4 ? 2 : 1;
    const style = l > 20 ? "width:25%;height:250px" :
                  l > 8 ? "width:30%;height:300px" :
                  l > 4 ? "width:45%;height:450px" :
                  "width:90%;height:900px";

    // HTML + dynamiczny <script type="module">
const base = window.location.origin + window.location.pathname.replace(/\/[^\/]*$/, '');
const fullPath = base + '/hal_js/modules/main.mjs';

const fullPaths = plotsTxt.map(p => {
    // zakładamy, że p to coś jak 'plots/hist_1.html'
    const rootPath = p.replace(".html", ".root");
    return `${base}/${rootPath}`;
});
magic_line = "const files = [\n";
for(let i=0; i<fullPaths.length; i++){
    magic_line += `  "${fullPaths[i]}"` + (i<fullPaths.length-1 ? ",\n" : "\n");
}
magic_line += "];\n";

    const html = `
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>QA Viewer</title>
</head>
<body>
  <div id="container"></div>
  <script type="module">
    import { openFile, draw, settings } from '${fullPath}';
    ${magic_line};
    settings.Palette = 50;
    let container = document.getElementById("container");
    let perRow = ${perRow};
    let style = "${style}";
    let row;
    for (let i = 0; i < files.length; i++) {
      if (i % perRow === 0) {
        row = document.createElement("div");
        row.style.display = "flex";
        container.appendChild(row);
      }
      let div = document.createElement("div");
      div.id = "drawing" + i;
      div.style = style;
      row.appendChild(div);

      const file = files[i].replace(".html", ".root");
      openFile(file).then(f => {
        f.readObject("canvas;1").then(obj => {
          draw(div.id, obj, "colz");
        });
      });
    }
  </script>
</body>
</html>
`;

    const blob = new Blob([html], { type: 'text/html' });
    const url = URL.createObjectURL(blob);
    window.open(url, '_blank');
}
