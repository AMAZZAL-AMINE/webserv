<?php
$diceNumber = mt_rand(1, 6);

$numText = "";

  switch($diceNumber) 
  {
  case 1:
    $numText = "One";
    break;
  case 2:
    $numText = "Two";
    break;
  case 3:
  case 4:
    $numText = "Three or Four";
    break;
  case 5:
    $numText = "Five";
    echo $numText;
  case 6:
    $numText = "Six";
    echo $numText;
    break;
  default:
    $numText = "unknown";
  }

  echo 'Dice show number '.$numText.'.';

?>