
#!/usr/bin/env python

import os, sys, Image

#usage:
#convert <xsize> <ysize> <old> <new>


class TileMap:
    """ This class represents a map of tiles.
    """
    def __init__(self, file, tileX, tileY):
        # For initialization, map image with filename file should be specified, together with the
        # tile size (tile X, tileY). First we set the tile sizes.
        self.TileX, self.TileY = tileX, tileY

        # Open the map and find its attributes.
        print "Opening the map image file: ", file
        self.MapImage = Image.open(file)
        self.MapImageWidth, self.MapImageHeight = self.MapImage.size
        self.Width, self.Height = self.MapImageWidth / self.TileX, self.MapImageHeight / self.TileY

        # Store the unique tiles in a list and a hash, and the map in a list.
        self.TileDict, self.IDDict = self.parseMap()

    def parseMap(self):
        IDDict = {}
        TDict = {}
        id_ = 0       

        for y in range(self.Height):
            for x in range(self.Width):
                box = self.TileX * x, self.TileY * y, self.TileX * (x+1), self.TileY * (y+1)
                tile = self.MapImage.crop(box)
                s = tile.tostring()

                TDict[s] = s
                IDDict[s] = id_
                id_ += 1
        return TDict, IDDict


if len(sys.argv) < 4:
    print "Error  : You specified too few arguments!\n"
else:
    tileX, tileY = int(sys.argv[1]), int(sys.argv[2])

    file1 = file(sys.argv[3])
    map1 = TileMap(file1, tileX, tileY)

    file2 = file(sys.argv[4])
    map2 = TileMap(file2, tileX, tileY)
   
    out = open("diff.txt_", "w")

    for tile1 in map1.TileDict:
        for tile2 in map2.TileDict:
            if map1.TileDict[tile1] == map2.TileDict[tile2]:
                if map1.IDDict[tile1] != map2.IDDict[tile2]:
                    print( map1.IDDict[tile1] , " >> " , map2.IDDict[tile2] )
                    out.write(str (map1.IDDict[tile1]) + " >> " + str (map2.IDDict[tile2]) + "\n")
    out.close()
    out = open("diff.txt_")
    lines = out.readlines()
    lines.sort()
    sorted = open("diff.txt", "w")
    map(sorted.write, lines)



