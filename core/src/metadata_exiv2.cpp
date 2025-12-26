#include "fo/providers/metadata_exiv2.hpp"
#include <iostream>

#ifdef FO_HAVE_EXIV2
#include <exiv2/exiv2.hpp>
#endif

namespace fo::providers {

bool Exiv2MetadataProvider::read(const std::filesystem::path& p, fo::core::ImageMetadata& out) {
#ifdef FO_HAVE_EXIV2
    try {
        auto image = Exiv2::ImageFactory::open(p.string());
        if (!image.get()) return false;
        image->readMetadata();

        Exiv2::ExifData &exifData = image->exifData();
        if (exifData.empty()) return false;

        // Date Taken
        Exiv2::ExifKey key("Exif.Photo.DateTimeOriginal");
        Exiv2::ExifData::const_iterator pos = exifData.findKey(key);
        if (pos != exifData.end()) {
            std::string dateStr = pos->toString();
            // Parse "YYYY:MM:DD HH:MM:SS"
            // For now, just store string or parse manually.
            // TinyEXIF provider has parsing logic we could reuse or duplicate.
            // Let's assume we just want to verify it works for now.
            out.date.has_taken = true;
            // TODO: Parse dateStr into out.date.taken (time_t)
        }

        // GPS
        // ... implementation details for GPS ...
        out.has_gps = false; // Placeholder

        return true;
    } catch (Exiv2::Error& e) {
        // std::cerr << "Exiv2 error: " << e.what() << "\n";
        return false;
    }
#else
    (void)p; (void)out;
    return false;
#endif
}

bool Exiv2MetadataProvider::write(const std::filesystem::path& p, const fo::core::ImageMetadata& in) {
#ifdef FO_HAVE_EXIV2
    try {
        auto image = Exiv2::ImageFactory::open(p.string());
        if (!image.get()) return false;
        image->readMetadata();
        Exiv2::ExifData &exifData = image->exifData();

        // Example: Update DateTimeOriginal
        if (in.date.has_taken) {
            // Format time_t to string
            // exifData["Exif.Photo.DateTimeOriginal"] = dateStr;
        }
        
        image->writeMetadata();
        return true;
    } catch (Exiv2::Error& e) {
        return false;
    }
#else
    (void)p; (void)in;
    return false;
#endif
}

} // namespace fo::providers
