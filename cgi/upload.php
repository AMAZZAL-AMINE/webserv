<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Uploads</title>
    <style>
        body
        {
            /* background-image: url('/./up.png'); */
            font-family: 'Arial', sans-serif;
            text-align: center;
        }

        h1 {
            color: #333;
        }

        .image-link {
            display: inline-block;
            padding: 10px 20px;
            margin: 10px;
            text-decoration: none;
            font-size: 16px;
            color: #fff;
            background-color: #3498db;
            border: 1px solid #2980b9;
            border-radius: 5px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            transition: transform 0.3s ease-in-out;
            cursor: pointer;
            overflow: hidden;
            perspective: 1000px;
        }

        .image-link:hover {
            transform: scale(1.1) rotateX(10deg);
        }
    </style>
</head>
<body>

<?php
$folderPath = '../html_root/uploads';

$images = glob($folderPath . '/*.{jpg,jpeg,png,gif,svg}', GLOB_BRACE);

if (count($images) > 0)
{
    echo "<h1>Uploaded Images</h1>";
    foreach ($images as $image)
    {
        $imageName = basename($image);
        $imageUrl = "./$imageName";
        echo "<a class='image-link' href='/$imageUrl'>$imageName</a><br>";
    }
}
else
{
    echo "<p>No images found in the specified folder.</p>";
}
?>

</body>
</html>