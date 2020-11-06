# Content Addressable Memory

Content Addressable Memory (abbr. CAM) is a special type of memory used for intensive search applications. This IP core is completely parameterizable in terms of data width and memory depth. CAM consists of three elements: block CAM, RAM with size DATA_WIDTH x MEMORY_DEPTH, and a lookup table of size (2^DATA_WIDTH) x MEMORY_DEPTH. The purpose of the RAM is to store the data, whereas the lookup table is used for getting the information whether the input data is contained in RAM.

Content Addressable Memory on [AppGallery](http://appgallery.maxeler.com/)
