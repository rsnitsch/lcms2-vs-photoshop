# lcms2-vs-photoshop

This repository was created in October 2021 to investigate a weird difference between the RGB-to-Lab colorspace conversions
in Adobe Photoshop versus the conversions calculated by Little-CMS engine. The problem is that Photoshop and Little-CMS
produce completely different results when black-point compensation is enabled.

For details please see this issue: https://github.com/mm2/Little-CMS/issues/285
