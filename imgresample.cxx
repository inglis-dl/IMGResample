#include <vtkImageChangeInformation.h>
#include <vtkImageData.h>
#include <vtkImageResample.h>
#include <vtkIMGReader.h>
#include <vtkIMGWriter.h>
#include <vtkNew.h>

int main(int argc, char** argv)
{
	if (argc != 4 || argc != 5 || argc != 7)
	{
		if (argc < 4)
		{
			std::cout << "Usage: IMGResample input.img output.img output_spacing" << std::endl;
			return EXIT_FAILURE;
		}
		else if (argc < 7 && argc > 5)
		{
			std::cout << "Usage: IMGResample input.img output.img output_x_spacing output_y_spacing" << std::endl;
			return EXIT_FAILURE;
		}
		else if (argc > 7)
		{
			std::cout << "Usage: IMGResample input.img output.img input_x_spacing input_y_spacing output_x_spacing output_y_spacing" << std::endl;
			return EXIT_FAILURE;
		}
	}

	std::string infile(argv[1]);

	if (infile.find_last_of(".") == std::string::npos)
		infile += ".img";

	std::string outfile(argv[2]);
	if (infile.find_last_of(".") == std::string::npos)
		outfile += ".img";

	double x_spacing_in = 1;
	double y_spacing_in = 1;
	double x_spacing_out = 1;
	double y_spacing_out = 1;
	if (argc == 4)
	{
		x_spacing_out = atof(argv[3]);
		y_spacing_out = atof(argv[3]);
	}
	else if (argc == 5)
	{
		x_spacing_out = atof(argv[3]);
		y_spacing_out = atof(argv[4]);
	}
	else if (argc == 7)
	{
		x_spacing_in = atof(argv[3]);
		y_spacing_in = atof(argv[4]);
		x_spacing_out = atof(argv[5]);
		y_spacing_out = atof(argv[6]);
	}
	else
	{
		std::cout << "Invalid input arguments" << std::endl;
	}

	if (x_spacing_in <= 0 || y_spacing_in <= 0)
	{
		std::cout << "Invalid input spacing" << std::endl;
		return EXIT_FAILURE;
	}
	if (x_spacing_out <= 0 || y_spacing_out <= 0)
	{
		std::cout << "Invalid output spacing" << std::endl;
		return EXIT_FAILURE;
	}

	vtkNew<vtkIMGReader> reader;
	reader->SetFileName(infile.c_str());
	if (!reader->CanReadFile(infile.c_str()))
	{
		std::cout << "Cannot read input file " << infile << std::endl;
		return EXIT_FAILURE;
	}

	vtkNew<vtkImageChangeInformation> change;
	change->SetOutputSpacing(x_spacing_in, y_spacing_in, 1);

	vtkNew<vtkIMGWriter> writer;
	writer->SetFileName(outfile.c_str());

	vtkNew<vtkImageResample> resample;
	resample->SetOutputSpacing(x_spacing_out, y_spacing_out, 1);
	resample->SetDimensionality(2);
	resample->SetInterpolationModeToLinear();
	resample->SetOutputScalarType(VTK_SHORT);

	change->SetInputConnection(reader->GetOutputPort());
	resample->SetInputConnection(change->GetOutputPort());
	writer->SetInputConnection(resample->GetOutputPort());

	writer->Write();

	int inDim[3];
	reader->GetOutput()->GetDimensions(inDim);
	int outDim[3];
	resample->GetOutput()->GetDimensions(outDim);
	std::cout << "input [" << inDim[0] << " x " << inDim[1] << "] => output [" << outDim[0] << " x " << outDim[1] << "]" << std::endl;

	return EXIT_SUCCESS;
}
