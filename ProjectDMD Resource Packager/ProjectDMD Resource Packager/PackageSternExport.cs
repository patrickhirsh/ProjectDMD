using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.IO;

/// <summary>
/// This class is designed to take a sequence of PNG images and generate quantized
/// versions of each image that fit in the Stern color space
/// </summary>
class PackageSternExport
{
    public static void Generate(string sourceDir)
    {
        foreach (string file in Directory.EnumerateFiles(sourceDir))
        {
            // obtain next image
            Bitmap image = new Bitmap(file);

        }
    }
}
