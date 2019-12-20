using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


static class PixelMapping
{
    /// <summary>
    /// Use this dictionary to map a PinExporter red-channel value to the corresponding
    /// 0-15 intensity value (per Stern spec).
	/// no mapping to 9 because 203 maps to both 6 and 9 per stern's spec... (can't have the same key map to 2 values)
    /// </summary>
    public static Dictionary<byte, byte> pinExporterMapping = new Dictionary<byte, byte>()
    {
        { 255, 15 },
        { 247, 14 },
        { 240, 13 },
        { 232, 12 },
        { 222, 11 },
        { 212, 10 },
        { 190, 8 },
        { 214, 7 },
        { 203, 6 },
        { 191, 5 },
        { 177, 4 },
        { 161, 3 },
        { 140, 2 },
        { 113, 1 },
        { 0, 0 },
    };
}
