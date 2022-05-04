<?php

// get js onclick for sorting
function getJavaSort($field)
{
    $addr = "index.php";
    $flag = "ASC";
    if (isset($_GET['flag'])) {
        if ($_GET['flag'] == "ASC") {
            $flag = "DESC";
        }
    }
    if (isset($_GET['tags'])) {
        $addr = "index.php?tags=" . $_GET['tags'] . "&flag=" . $flag;
    } else {
        $addr = "index.php?flag=" . $flag;
    }
    $addr = $addr . "&orderby=" . $field;
    return "<td onclick=\"window.location='" . $addr . "'\">";
}

// return query with or without sorting
function getQuery($db)
{
    $stmt = "SELECT * FROM Files";
    if (isset($_GET['tags'])) {
        if ($_GET['tags'] != "") {
            $que = $_GET['tags'];

            preg_match_all('("([^"]|"")*")', $que, $sentences);
            foreach ($sentences[0] as $value) {
                $que = str_replace($value, " ", $que);
            }
            $que = preg_replace('/\s+/', ' ', $que);
            $arr = preg_split('/[\s]+/', $que);
            $i = 0;
            $bind_tags = "";
            $bind_comment = "";
            $array_tags = array();
            $array_comment = array();
            foreach ($arr as $value) {
                if (strlen($value) == 0)
                    continue;
                if ($value[0] == '#') {
                    $value = substr($value, 1); // remove #
                    array_push($array_tags, $value);
                } else {
                    array_push($array_comment, $value);
                }
            }
            $question_tag = "";
            $name_id = 0;
            if (count($array_tags) > 0 || count($array_comment) > 0 || count($sentences[0]) > 0) {
                $stmt = $stmt . " WHERE ";
            }
            if (count($array_tags) > 0) {
                $question_tag = "Tags LIKE ";
                foreach ($array_tags as $value) {
                    $question_tag = $question_tag . ":name" . ($name_id ++) . " AND Tags LIKE";
                }
                $question_tag = substr($question_tag, 0, - 13);
                $question_tag = $question_tag . "";
            }
            $question_word = "";
            if (count($array_comment) > 0 || count($sentences[0]) > 0) {
                $question_word = "Comment LIKE ";
                foreach ($array_comment as $value) {
                    $question_word = $question_word . ":name" . ($name_id ++) . " AND Comment LIKE";
                }
                foreach ($sentences[0] as $value) {
                    $question_word = $question_word . ":name" . ($name_id ++) . " AND Comment LIKE";
                }
                $question_word = substr($question_word, 0, - 16);
                $question_word = $question_word . "";
            }

            if (strlen($question_tag) > 0 && strlen($question_word) > 0) {
                $stmt = $stmt . "(" . $question_tag . ") AND (" . $question_word . ")";
            } else if (strlen($question_tag) > 0) {
                $stmt = $stmt . " " . $question_tag;
            } else {
                $stmt = $stmt . " " . $question_word;
            }
            $statement = $db->prepare($stmt);
            $count = 0;
            foreach ($array_tags as $val) {
                $val = '%' . $val . '%';
                $parname = ":name" . $count ++;
                $statement->bindParam($parname, $val);
            }
            foreach ($array_comment as $val) {
                $val = "%" . $val . "%";
                $parname = ":name" . $count ++;
                $statement->bindParam($parname, $val);
            }
            foreach ($sentences[0] as $val) {
                $val = "%" . $val . "%";
                $parname = ":name" . $count ++;
                $statement->bindParam($parname, $val);
            }

            $xx = $statement->execute();
            // $statement->debugDumpParams();
            return $xx;
        }
    }
    if (isset($_GET['flag']) && isset($_GET['orderby'])) {
        $stmt = $stmt . " ORDER BY " . $_GET['orderby'] . " " . $_GET['flag'];
        return $db->query($stmt);
    }
    return $db->query($stmt);
}

// return google record
function getGoogleRec($row)
{
    echo "<a href=\"html_rep/html_" . $row['FILE_ID'] . "/index.html?task_id=" . $row['TASK_ID'] . "\">" . $row['ID'] . ". " . $row['Comment'] . "</a></br>" . PHP_EOL;
    echo "<font color=\"green\">Tags : " . $row['Tags'] . "</font></br>" . PHP_EOL;
    echo "File path: " . $row['PathFile'] . "</br>" . PHP_EOL;
    echo "Analysis name: " . $row['AnaName'] . "</br>" . PHP_EOL;
    echo "Input file: " . $row['InputFile'] . "</br>" . PHP_EOL;
    echo "Data format: " . $row['DataType'] . "</br>" . PHP_EOL;
    echo "Processed events: " . $row['ProcessedEvents'] . "</br>" . PHP_EOL;
    echo "Software ver: " . $row['SoftVer'] . "</br>" . PHP_EOL;
}

// get table-like record
function getTableRec($row)
{
    echo "<tr><td>" . $row['ID'] . "</td><td>" . $row['Comment'] . "</td><td>" . $row['InputFile'] . "</td><td>" . $row['AnaName'] . "</td><td>" . $row['PathFile'] . "</td><td><a href=\"html_rep/html_" . $row['FILE_ID'] . "/index.html?task_id=" . $row["TASK_ID"] . "\">Link</a></td><td>" . $row['Tags'] . "</td></tr>" . PHP_EOL;
}

// show entrie table
function showTable($result)
{
    echo "</td></tr></table>" . PHP_EOL;
    echo "<div class=\"CSSTableGenerator\">" . PHP_EOL;
    echo "<table class=\"sortable\">" . PHP_EOL;
    echo "<tr><td>No</td>" . getJavaSort("Comment") . "Comment</td>" . getJavaSort("InputFile") . "InputFile</td>" . getJavaSort("AnaName") . "AnaName</td>" . getJavaSort("PathFile") . "Full Path</td>" . getJavaSort("ID") . "Link</td>" . getJavaSort("Tags") . "Tags</td><tr>" . PHP_EOL;
    while ($row = $result->fetchArray(SQLITE3_ASSOC)) {
        getTableRec($row);
    }
    echo "</table>" . PHP_EOL;
    echo "</div>" . PHP_EOL;
}

// show google
function showGoogle($result)
{
    while ($row = $result->fetchArray(SQLITE3_ASSOC)) {
        getGoogleRec($row);
    }
    echo "</table>" . PHP_EOL;
}

?>