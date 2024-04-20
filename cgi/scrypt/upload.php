
<title>Uploads</title>
<style>
        body {
            position: relative;
            font-family: 'Arial', sans-serif;
            text-align: center;
            margin: 0;
            padding: 0;
            color: #fff;
            overflow: hidden;
        }

        body::before {
            content: "";
            position: absolute;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background: linear-gradient(to bottom right, #744136, #8e44ad);
            filter: blur(100px);
        }

        h1 {
            margin: 100px;
            color: #000;
            font-family: 'Arial', sans-serif;

        }

        .image-link {
            display: inline-block;
            text-decoration: none;
            transition: transform 0.3s ease-in-out;
            cursor: pointer;
            overflow: hidden;
            perspective: 1000px;
            margin: 10px;
        }

        .image-link:hover {
            transform: scale(1.1) rotateX(10deg);
        }

        .image-thumbnail {
            width: 150px;
            height: 150px;
            border-radius: 5px;
            object-fit: cover;
        }

        .image-info {
            display: block;
            color: #fff;
        }
    </style>

<?php
$folderPath = '../../html_root/assets';
$images = glob($folderPath . '/*.*', GLOB_NOSORT);

// Set the number of images to display in each set
$imagesPerSet = 4;

if (count($images) > 0)
{
    echo "<h1>Uploaded Images</h1>";
    if (count($images) > 0)
    {
        for ($i = 0; $i < count($images); $i += $imagesPerSet)
        {
            echo "<div>";
            for ($j = $i; $j < min($i + $imagesPerSet, count($images)); $j++)
            {
                $image = $images[$j];
                $imageName = basename($image);
                $imageUrl = "./$imageName";
                
                echo "<a class='image-link' href='/$imageUrl'>";
                echo "<img class='image-thumbnail' src='/$imageUrl' alt='$imageName'>";
                echo "<span class='image-info'>$imageName</span>";
                echo "</a>";
            }
            echo "</div>";
        }
    }
}
else
{
    echo "<p>No images found in the specified folder.</p>";
}
?>



<!-- echo '<img style="object-fit: cover;" src="' . $imageUrl . '" alt="Image">'; -->
<!-- echo '<img src="' . $imageUrl . '" alt="Image">'; -->