#include "base/task/CancellationTokenSource.h"
#include "CLI/CLI.hpp"
#include "ffmpeg-wrapper/mux/AVMixer.h"
#include "ffmpeg-wrapper/output-format/FileOutputFormat.h"

int main(int argc, char **argv)
{
	try
	{
		std::string video_url;
		std::string audio_url;
		std::string output_file_path;

		CLI::App app{"将不同来源的一路视频流和一路音频流混合在一起。"};

		app.add_option("--video_url",
					   video_url,
					   "视频源的 url")
			->required();

		app.add_option("--audio_url",
					   audio_url,
					   "音频源的 url")
			->required();

		app.add_option("--output_file_path",
					   output_file_path,
					   "输出文件的路径。输出文件推荐使用 ts，除非你能确保音频流和视频流的持续时间相同且起始时间相同。")
			->required();

		// 解析命令行参数
		CLI11_PARSE(app, argc, argv);

		auto start = std::chrono::high_resolution_clock::now();

		std::shared_ptr<video::InputFormat> input_video_format{new video::InputFormat{video_url}};
		std::shared_ptr<video::InputFormat> input_audio_format{new video::InputFormat{audio_url}};
		std::shared_ptr<video::FileOutputFormat> output_format{new video::FileOutputFormat{output_file_path}};
		video::AVMixer mix{input_video_format, input_audio_format, output_format};
		base::CancellationTokenSource cancel_pump_source;
		mix.PumpDataToConsumers(cancel_pump_source.Token());

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "花费的时间: " << duration.count() << "ms" << std::endl;
		return 0;
	}
	catch (std::runtime_error const &e)
	{
		std::cout << e.what() << std::endl;
		throw;
	}
}
