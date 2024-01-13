<?php
$folderPath = '../html_root/assets';

$images = glob($folderPath . '/*.{jpg,jpeg,png,gif,svg}', GLOB_BRACE);

if (count($images) > 0)
{
    echo "<h1>Uploaded Images</h1>";
    echo count($images);
    echo '<br>';
    foreach ($images as $image)
    {
        $imageName = basename($image);
        $imageUrl = "./$imageName";
        echo "<a href='/$imageUrl'>$imageName</a><br>";
    }
}
else
{
    echo "<p>No images found in the specified folder.</p>";
}
?>