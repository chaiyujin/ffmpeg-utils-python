#include "ffutils/ffutils.hpp"

static void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
    FILE *pFile;
    char szFilename[32];
    int  y;
    
    // Open file
    sprintf(szFilename, "frames/%d.ppm", iFrame);

    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
        return;
    
    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);
    
    // Write pixel data
    for(y=0; y<height; y++)
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
    
    // Close file
    fclose(pFile);
}

int main(int argc, char *argv[]) {
    // log::set_level(log::level::debug);

    ffutils::VideoReader reader;
    ffutils::MediaConfig cfg;
    // cfg.video.resolution = {320, 180};
    if (!reader.open(argv[1], cfg)) {
        log::error("Failed to open video!");
    }

    bool got = false;

    // if (reader.is_open()) {
    //     log::info("n_frames: {}", reader.n_frames());
    //     for (int32_t tar = 0; tar < 10; ++tar)
    //     {
    //         Timeit _(fmt::format("seek frame {}", tar));
    //         reader.seek(tar);
    //         log::info("duration: {}", reader.duration());
    //         // SaveFrame(reader.frame(), reader.frame()->width, reader.frame()->height, tar);
    //     }
    //     log::info("n_frames: {}", reader.n_frames());
    //     log::info("current timestamp: {}", reader.current_timestamp());
    // }

    if (reader.is_open()) {
        log::info("n_frames: {}", reader.n_frames());
        for (int32_t tar = 0; tar < 10; ++tar) {
            fmt::print("test\n");
            Timeit _(fmt::format("seek frame {}", tar));
            reader.read();
            SaveFrame(reader.frame(), reader.frame()->width, reader.frame()->height, tar);
        }
        reader.seek(100);
        for (int32_t tar = 0; tar < 10; ++tar) {
            Timeit _(fmt::format("seek frame {}", tar + 100));
            reader.read();
            SaveFrame(reader.frame(), reader.frame()->width, reader.frame()->height, tar + 100);
        }
        log::info("n_frames: {}", reader.n_frames());
        log::info("current timestamp: {}", reader.current_timestamp());
    }

    return 0;
}
