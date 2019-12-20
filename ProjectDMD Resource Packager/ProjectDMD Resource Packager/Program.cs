using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

public enum ResourceType { NONE, DMDF, DMDA }


class Program
{
    static ResourceType mode = ResourceType.NONE;
    static string sourceDir = "..\\..\\source";
    static string name = "";
    static byte framerate = 0;

    static string USAGE = @"
==================== Resource Packager Usage ====================
Generate DMD resource files from a collection of source images in the
provided source directory.

This Tool is designed to be used with images generated from PinExporter.
Images in sourceDir MUST be images generated with PinExporter. These images
use special red-channel values (per Stern spec) that allow simple packaging
into the .dmdf format.


** MODES **

    DMDF: Generates a .dmdf font file.
images must be named the character that they represent. This information is 
used in packaging to make unpacking and mapping within ProjectDMD easy.

    DMDA: Generates a .dmda animation file.
images must be named numerical values 0-n in order to correctly construct the 
animation sequence. All frames must be the same dimensions.

    SternExport: Given a series of PNG images, generates the same series of
images, quantized and reduced to fit in the Stern spec colorspace


** ARGUMENTS **

-help                    : display usage
-mode=<mode>             : the resource type to generate (one of the above modes)
-source=<sourceDir>      : the source directory path (relative)
-name=<outputFileName>   : name of the output file
-framerate=<framerate>   : framerate (in fps). DMDA only.

";

    static void Main(string[] args)
    {
        // parse args
        foreach (string arg in args)
        {
            if (arg == "-help")
            {
                Console.Write(USAGE);
                return;
            }

            string[] splitArg = arg.Split('=');
            if (splitArg.Length == 2)
            {
                switch(splitArg[0].ToLower())
                {
                    // ALL
                    case "-source":
                        sourceDir = splitArg[1];
                        break;
                    case "-name":
                        name = splitArg[1];
                        break;
                    case "-mode":
                        if (splitArg[1].ToLower() == "dmdf")
                            mode = ResourceType.DMDF;
                        if (splitArg[1].ToLower() == "dmda")
                            mode = ResourceType.DMDA;
                        break;

                    // DMDA
                    case "-framerate":
                        framerate = (byte)int.Parse(splitArg[1]);
                        break;

                    default:
                        Console.WriteLine("ERROR: unrecognized argument: " + splitArg[0]);
                        break;
                }
            }
        }

        switch(mode)
        {
            case ResourceType.DMDF:
                if (!validateArgsDMDF()) { return; }
                PackageDMDF.Generate(sourceDir, name);
                break;
            case ResourceType.DMDA:
                if (!validateArgsDMDA()) { return; }
                PackageDMDA.Generate(sourceDir, name, framerate);
                break;
            case ResourceType.NONE:
                // intentional fall-through
            default:
                Console.WriteLine("ERROR: please provide a valid mode.");
                break;
        }
    }

    /// <summary>
    /// Arguments applicable to all resource types
    /// </summary>
    static bool validateArgsALL()
    {
        if (!Directory.Exists(sourceDir))
        {
            Console.WriteLine("ERROR: invalid source directory provided: " + sourceDir);
            Console.WriteLine(USAGE);
            return false;
        }
        if (name == "")
        {
            Console.WriteLine("ERROR: an output filename wasn't provided...");
            Console.WriteLine(USAGE);
            return false;
        }

        return true;
    }

    /// <summary>
    /// DMDF-Specific arguments
    /// </summary>
    static bool validateArgsDMDF()
    {
        if (!validateArgsALL()) { return false; }

        return true;
    }

    /// <summary>
    /// DMDA-specifc arguments
    /// </summary>
    static bool validateArgsDMDA()
    {
        if (!validateArgsALL()) { return false; }

        if (framerate == 0)
        {
            Console.WriteLine("ERROR: a framerate must be provided for DMDA generation");
            Console.WriteLine(USAGE);
            return false;
        }

        return true;
    }
}
