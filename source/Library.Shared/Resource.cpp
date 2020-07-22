#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Resource.h"
#pragma endregion Includes

namespace Library
{
    static const long Vertex = FormatFlags::SupportsVertex;
    static const long Mips = FormatFlags::SupportsMips;
    static const long Packed = FormatFlags::IsPacked;
    static const long Compressed = FormatFlags::IsCompressed;
    static const long RenderTarget = FormatFlags::SupportsRenderTarget;
    static const long Depth = FormatFlags::HasDepth;
    static const long Stencil = FormatFlags::HasStencil;
    static const long GenMips = FormatFlags::SupportsGenerateMips | Mips | RenderTarget;
    static const long sRGB = FormatFlags::IsColorSpace_sRGB;

	static const long Dim1D_2D = FormatFlags::SupportsTexture1D | FormatFlags::SupportsTexture2D;
    static const long Dim2D_3D = FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D;
    static const long Dim1D_2D_3D = FormatFlags::SupportsTexture1D | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D;
    static const long DimCube = FormatFlags::SupportsTextureCube;
	
    static const long UInt = FormatFlags::IsUnsignedInteger;
    static const long SInt = FormatFlags::IsInteger;
    static const long UNorm = UInt | FormatFlags::IsNormalized;
    static const long SNorm = SInt | FormatFlags::IsNormalized;
	
    const HashMap<Format, FormatDesc> FormatDescMap =
    {
        { Format::Unknown,          { 0, 0, 0, 0,   0                                                                   }   },

    	{ Format::A8UNorm,          { 8, 1, 1, 1,   GenMips | Dim1D_2D_3D | DimCube | UNorm                             }   },

        { Format::R8UNorm,          { 8, 1, 1, 1,   Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm                    }   },
        { Format::R8SNorm,          { 8, 1, 1, 1,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm                    }   },
        { Format::R8UInt,           { 8, 1, 1, 1,   Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::R8SInt,           { 8, 1, 1, 1,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },

        { Format::R16UNorm,         { 16, 1, 1, 1,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm                    }   },
        { Format::R16SNorm,         { 16, 1, 1, 1,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm                    }   },
        { Format::R16UInt,          { 16, 1, 1, 1,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::R16SInt,          { 16, 1, 1, 1,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },
        { Format::R16Float,         { 16, 1, 1, 1,  Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::R32UInt,          { 32, 1, 1, 1,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::R32SInt,          { 32, 1, 1, 1,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },
        { Format::R32Float,         { 32, 1, 1, 1,  Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::R64Float,         { 64, 1, 1, 1,  Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::RG8UNorm,         { 16, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm                    }   },
        { Format::RG8SNorm,         { 16, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm                    }   },
        { Format::RG8UInt,          { 16, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::RG8SInt,          { 16, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },

        { Format::RG16UNorm,        { 32, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm                    }   },
        { Format::RG16SNorm,        { 32, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm                    }   },
        { Format::RG16UInt,         { 32, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::RG16SInt,         { 32, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },
        { Format::RG16Float,        { 32, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::RG32UInt,         { 64, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::RG32SInt,         { 64, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },
        { Format::RG32Float,        { 64, 1, 1, 2,  Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::RG64Float,        { 128, 1, 1, 2, Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::RGB8UNorm,        { 24, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm                    }   },
        { Format::RGB8UNorm_sRGB,   { 24, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm | sRGB             }   },
        { Format::RGB8SNorm,        { 24, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm                    }   },
        { Format::RGB8UInt,         { 24, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::RGB8SInt,         { 24, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },

        { Format::RGB16UNorm,       { 48, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm                    }   },
        { Format::RGB16SNorm,       { 48, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm                    }   },
        { Format::RGB16UInt,        { 48, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::RGB16SInt,        { 48, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },
        { Format::RGB16Float,       { 48, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::RGB32UInt,        { 96, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::RGB32SInt,        { 96, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },
        { Format::RGB32Float,       { 96, 1, 1, 3,  Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::RGB64Float,       { 192, 1, 1, 3, Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::RGBA8UNorm,       { 32, 1, 1, 4,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm                    }   },
        { Format::RGBA8UNorm_sRGB,  { 32, 1, 1, 4,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm | sRGB             }   },
        { Format::RGBA8SNorm,       { 32, 1, 1, 4,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm                    }   },
        { Format::RGBA8UInt,        { 32, 1, 1, 4,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::RGBA8SInt,        { 32, 1, 1, 4,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },

        { Format::RGBA16UNorm,      { 64, 1, 1, 4,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm                    }   },
        { Format::RGBA16SNorm,      { 64, 1, 1, 4,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm                    }   },
        { Format::RGBA16UInt,       { 64, 1, 1, 4,  Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::RGBA16SInt,       { 64, 1, 1, 4,  Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },
        { Format::RGBA16Float,      { 64, 1, 1, 4,  Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::RGBA32UInt,       { 128, 1, 1, 4, Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt                     }   },
        { Format::RGBA32SInt,       { 128, 1, 1, 4, Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt                     }   },
        { Format::RGBA32Float,      { 128, 1, 1, 4, Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::RGBA64Float,      { 256, 1, 1, 4, Vertex | GenMips | Dim1D_2D_3D | DimCube                            }   },

        { Format::BGRA8UNorm,       { 32, 1, 1, 4,  GenMips | Dim1D_2D_3D | DimCube | UNorm                             }   },
        { Format::BGRA8UNorm_sRGB,  { 32, 1, 1, 4,  GenMips | Dim1D_2D_3D | DimCube | UNorm | sRGB                      }   },
        { Format::BGRA8SNorm,       { 32, 1, 1, 4,  GenMips | Dim1D_2D_3D | DimCube | SNorm                             }   },
        { Format::BGRA8UInt,        { 32, 1, 1, 4,  GenMips | Dim1D_2D_3D | DimCube | UInt                              }   },
        { Format::BGRA8SInt,        { 32, 1, 1, 4,  GenMips | Dim1D_2D_3D | DimCube | SInt                              }   },

        { Format::RGB10A2UNorm,     { 32, 1, 1, 4,  GenMips | Dim1D_2D_3D | DimCube | UNorm | Packed                    }   },
        { Format::RGB10A2UInt,      { 32, 1, 1, 4,  GenMips | Dim1D_2D_3D | DimCube | SInt  | Packed                    }   },
        { Format::RG11B10Float,     { 32, 1, 1, 3,  GenMips | Dim1D_2D_3D | DimCube | UNorm | Packed                    }   },
        { Format::RGB9E5Float,      { 32, 1, 1, 3,  Mips    | Dim1D_2D_3D | DimCube | Packed                            }   },

        { Format::D16UNorm,         { 16, 1, 1, 1,  Mips | RenderTarget | Dim1D_2D | DimCube | UNorm | Depth            }   },
        { Format::D24UNormS8UInt,   { 32, 1, 1, 2,  Mips | RenderTarget | Dim1D_2D | DimCube | UNorm | Depth | Stencil  }   },
        { Format::D32Float,         { 32, 1, 1, 1,  Mips | RenderTarget | Dim1D_2D | DimCube | Depth                    }   },
        { Format::D32FloatS8X24UInt,{ 64, 1, 1, 2,  Mips | RenderTarget | Dim1D_2D | DimCube | Depth | Stencil          }   },
        { Format::S8UInt,           { 8, 1, 1, 1,   Mips | RenderTarget | Dim1D_2D | DimCube | UInt  | Stencil          }   },

        { Format::BC1UNorm,         { 64, 4, 4, 4,  Mips | Dim2D_3D | DimCube | Compressed | UNorm                      }   },
        { Format::BC1UNorm_sRGB,    { 64, 4, 4, 4,  Mips | Dim2D_3D | DimCube | Compressed | UNorm | sRGB               }   },
        { Format::BC2UNorm,         { 128, 4, 4, 4, Mips | Dim2D_3D | DimCube | Compressed | UNorm                      }   },
        { Format::BC2UNorm_sRGB,    { 128, 4, 4, 4, Mips | Dim2D_3D | DimCube | Compressed | UNorm | sRGB               }   },
        { Format::BC3UNorm,         { 128, 4, 4, 4, Mips | Dim2D_3D | DimCube | Compressed | UNorm                      }   },
        { Format::BC3UNorm_sRGB,    { 128, 4, 4, 4, Mips | Dim2D_3D | DimCube | Compressed | UNorm                      }   },
        { Format::BC4UNorm,         { 64, 4, 4, 1,  Mips | Dim2D_3D | DimCube | Compressed | UNorm | sRGB               }   },
        { Format::BC4SNorm,         { 64, 4, 4, 1,  Mips | Dim2D_3D | DimCube | Compressed | SNorm                      }   },
        { Format::BC5UNorm,         { 128, 4, 4, 2, Mips | Dim2D_3D | DimCube | Compressed | UNorm                      }   },
        { Format::BC5SNorm,         { 128, 4, 4, 2, Mips | Dim2D_3D | DimCube | Compressed | SNorm                      }   }
    };
}
