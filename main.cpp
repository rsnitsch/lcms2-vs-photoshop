#include <iostream>
#include <string>
#include <vector>

//#include <opencv2/opencv.hpp>
#include <lcms2.h>

using std::cout;
using std::endl;
using std::vector;

struct SampleColor {
    std::string name;
    double R;
    double G;
    double B;
};

int main(int argc, char** argv) {
    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }

    cmsHPROFILE hInProfile = cmsOpenProfileFromFile("InputProfile20211015.icc", "r");
    if (hInProfile == nullptr) {
        std::cout << "Error: Could not load input profile" << std::endl;
        return 1;
    }

    cmsHPROFILE hOutProfile = cmsCreateLab4Profile(nullptr);

    // cmsFLAGS_HIGHRESPRECALC
    // cmsFLAGS_NONEGATIVES
    // cmsFLAGS_BLACKPOINTCOMPENSATION
    int intent = INTENT_RELATIVE_COLORIMETRIC;

    // TODO: With or without makes no difference but in Photoshop it does?!
    //int flags = 0; // Without.
    int flags = cmsFLAGS_BLACKPOINTCOMPENSATION; // With.

    cmsHTRANSFORM transform = cmsCreateTransform(hInProfile, TYPE_RGB_DBL, hOutProfile,
        TYPE_Lab_DBL,
        intent, flags);

    cmsCloseProfile(hInProfile);
    cmsCloseProfile(hOutProfile);

    // RGB values from the color checker chart.
    std::vector<SampleColor> sampleColors = {
        {"brown", 165, 127, 88         },
        {"skin", 217, 166, 121         },
        {"light blue", 116, 138, 198   },
        {"green", 122, 151, 71         },
        {"violet", 151, 136, 203       },
        {"turquoise", 127, 204, 178    },
        {"orange", 241, 150, 1         },
        {"blue", 79, 80, 225           },
        {"light red", 231, 93, 90      },
        {"violet", 119, 69, 144        },
        {"light green", 173, 217, 31   },
        {"ocker", 227, 178, 1          },
        {"blue", 52, 39, 215           },
        {"green", 113, 204, 77         },
        {"red", 219, 65, 36            },
        {"yellow", 239, 224, 1         },
        {"pink", 223, 86, 175          },
        {"light blue", 61, 152, 209    },
        {"white", 250, 252, 241        },
        {"gray", 224, 225, 217         },
        {"gray", 203, 206, 197         },
        {"gray", 167, 168, 159         },
        {"gray", 126, 128, 123         },
        {"black", 66, 67, 62           }
    };

    double RGB[3];
    cout << "Field,\tName,\tL,\ta,\tb" << endl;
    for (int i = 0; i < sampleColors.size(); ++i) {
        auto sampleColor = sampleColors[i];
        RGB[0] = sampleColor.R / 255.0; RGB[1] = sampleColor.G / 255.0; RGB[2] = sampleColor.B / 255.0;

        cmsCIELab Lab;
        cmsDoTransform(transform, RGB, &Lab, 1);

        cout << i+1 << ",\t" << sampleColor.name << ",\t" << Lab.L << ",\t" << Lab.a << ",\t" << Lab.b << endl;
    }

    cmsDeleteTransform(transform);
    return 0;
}
