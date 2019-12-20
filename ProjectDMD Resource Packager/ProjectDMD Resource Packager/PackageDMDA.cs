using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.IO;

/// <summary>
/// 
/// This class is designed to take a sequence of PNG images and generate a .dmda file
/// 
/// DMDF FILE FORMAT:
/// 
/// HEADER
/// CONTENT
/// 
/// HEADER FORMAT:
/// [0] -   byte framerate (frames per second)
/// [1-4] -   int frame width
/// [5-8] -   int frame height
/// 
/// CONTENT FORMAT:
///     A series of bytes:
///     0-15 = intensity
///     255 = transparent
/// 
/// 
/// *** NOTE ***
/// This class is designed to be used with images generated from PinExporter.
/// Images in sourceDir MUST be images generated with PinExporter. These images
/// use special red-channel values (per Stern spec) that allow simple packaging
/// into the .dmda format. Furthermore, images must be named numerical values 0-n
/// in order to correctly construct the animation sequence. All frames must be
/// the same dimensions.
/// </summary>
class PackageDMDA
{
    public static void Generate(string sourceDir, string name, byte framerate)
    {
        // create file and get list of source files
        FileStream output = File.Create(name + ".dmda");
        List<string> files = Directory.EnumerateFiles(sourceDir).OrderBy(x => x).ToList();
        if (files.Count == 0) { return; }

        // write header
        Bitmap image = new Bitmap(files[0]);
        int width = image.Width;
        int height = image.Height;
        byte[] header = new byte[]
        {
                    framerate,
                    (byte) width,
                    (byte) (width >> 8),
                    (byte) (width >> 16),
                    (byte) (width >> 24),
                    (byte) height,
                    (byte) (height >> 8),
                    (byte) (height >> 16),
                    (byte) (height >> 24)
        };
        output.Write(header, 0, header.Length);

        foreach (string file in files)
        {
            // write payload
            int payloadIndex = 0;
            byte[] payload = new byte[image.Width * image.Height];
            for (int row = 0; row < image.Height; row++)
            {
                for (int col = 0; col < image.Width; col++)
                {
                    Color pixel = image.GetPixel(col, row);
                    if (pixel.A == 0)
                        payload[payloadIndex] = 255;
                    else if (PixelMapping.pinExporterMapping.ContainsKey(pixel.R))
                        payload[payloadIndex] = PixelMapping.pinExporterMapping[pixel.R];
                    else { throw new Exception("ERROR: Invalid image data"); }
                    payloadIndex++;
                }
            }
            output.Write(payload, 0, payload.Length);
        }
        output.Close();
    }
}
