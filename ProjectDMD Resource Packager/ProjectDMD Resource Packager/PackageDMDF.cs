using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.IO;


/// <summary>
/// 
/// This class is designed to take a sequence of PNG images and generate a .dmdf file
/// Maximum image size: 255*255
/// 
/// DMDF FILE FORMAT:
///     ...
/// HEADER ...
/// CONTENT ...
/// HEADER ...
/// CONTENT ...
/// etc.
///     ...
/// 
/// HEADER FORMAT:
/// [0] -   char character
/// [1] -   byte width
/// [2] -   byte height
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
/// into the .dmdf format. Furthermore, images must be named the character that
/// they represent. This information is used in packaging to make unpacking and
/// mapping within ProjectDMD easy.
/// </summary>
static class PackageDMDF
{
    /* Certain special characters can't be used as file names, so this map contains code names 
     used to represent these characters. Follow this mapping when created font sources to include
     these special characters. */
    static Dictionary<string, char> SPECIAL_CHARACTER_MAPPING = new Dictionary<string, char>()
    {
        {"ASTERISK", '*' },
        {"BSLASH", '\\' },
        {"COLON", ':' },
        {"FSLASH", '/' },
        {"LBRACK", '<' },
        {"PERIOD", '.' },
        {"PIPE", '|' },
        {"QUESTION", '?' },
        {"RBRACK", '>' },
        {"SEMICOLON", ';' },
        {"SPACE", ' ' },
    };

    public static void Generate(string sourceDir, string name)
    {
        // create file and enumerate through source files
        FileStream output = File.Create(name + ".dmdf");
        foreach (string file in Directory.EnumerateFiles(sourceDir))
        {
            // obtain next image
            Bitmap image = new Bitmap(file);

            // obtain character
            char character;
            if (Path.GetFileNameWithoutExtension(file).Length > 1)
            {
                if (SPECIAL_CHARACTER_MAPPING.ContainsKey(Path.GetFileNameWithoutExtension(file)))
                    character = SPECIAL_CHARACTER_MAPPING[Path.GetFileNameWithoutExtension(file)];
                else { throw new Exception("ERROR: Unknown character detected: " + Path.GetFileNameWithoutExtension(file)); }
            }
            else { character = Path.GetFileNameWithoutExtension(file)[0]; }

            // build header
            byte width = (byte)image.Width;
            byte height = (byte)image.Height;
            byte[] header = new byte[] { (byte)character, width, height };
            output.Write(header, 0, header.Length);

            // write payload
            int payloadIndex = 0;
            byte[] payload = new byte[image.Width * image.Height];
            for (int row = 0; row < image.Height; row++)
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
            output.Write(payload, 0, payload.Length);
        }
        output.Close();
    }
}
