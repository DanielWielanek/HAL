<head>
<link rel="stylesheet" type="text/css" href="search_template.css">
</head>
<body style="margin: 0; padding: 0">
	<table align="center">
		<tr>
			<td><img src="logo_search.gif" alt="Search" height="92" width="420">
				<form method="get" action="index.php">
Search by tags:
<?php
if (isset($_GET['tags'])) {
    echo "<input type=\"text\" name=\"tags\" value=\"" . $_GET['tags'] . "\">";
} else {
    echo "<input type=\"text\" name=\"tags\">";
}
include ('functions.php');
?>

<input type="submit" value="Search">
				</form>

<?php
$db = new SQLite3('database.sqlite');
// $stmt = getQuery();
$result = getQuery($db);

// $db->query($stmt);
if (isset($_GET['tags'])) {
    if ($_GET['tags'] != "" && count($result) < 21) {
        showGoogle($result);
    } else {
        showTable($result);
    }
} else {
    showTable($result);
}
$db->Close();
?>