/*=========================================================================

  Module:    vtkIMGReader.cxx
  Program:
  Language:  C++
  Author:    Dean Inglis <inglisd AT gmail DOT com>

=========================================================================*/
#include <vtkIMGReader.h>

#include <vtkImageData.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <sys/stat.h>

vtkStandardNewMacro(vtkIMGReader);

#pragma warning(disable : 4996)

//----------------------------------------------------------------------------
vtkIMGReader::vtkIMGReader()
{
	this->FileLowerLeft = 1;
}

//----------------------------------------------------------------------------
void vtkIMGReader::ExecuteInformation()
{
	// if the user has not set the extent, but has set the VOI
	// set the zaxis extent to the VOI z axis
	if (this->DataExtent[4] == 0 && this->DataExtent[5] == 0 &&
		(this->DataVOI[4] || this->DataVOI[5]))
	{
		this->DataExtent[4] = this->DataVOI[4];
		this->DataExtent[5] = this->DataVOI[5];
	}

	if (!this->FileName && !this->FilePattern)
	{
		vtkErrorMacro(<< "Either a FileName or FilePattern must be specified.");
		return;
	}

	// Allocate the space for the filename
	this->ComputeInternalFileName(this->DataExtent[4]);

	FILE* fp = fopen(this->InternalFileName, "rb");
	if (!fp)
	{
		vtkErrorMacro("Unable to open file " << this->InternalFileName);
		return;
	}

	struct stat statbuf;
	fstat(fileno(fp), &statbuf);

	char buff[8];
	fread(buff, 1, 8, fp);
	int xsize;
	int ysize;
	fread((void*)&xsize, 1, 4, fp);
	fread((void*)&ysize, 1, 4, fp);

	int header = static_cast<int>(statbuf.st_size - xsize * ysize * 2);

	// close the file
	fclose(fp);

	if (header != 16)
	{
		vtkErrorMacro(<< "Unknown file type! Not an IMG file!");
		return;
	}

	this->SetHeaderSize(header);

	// if the user has set the VOI, just make sure its valid
	if (this->DataVOI[0] || this->DataVOI[1] ||
		this->DataVOI[2] || this->DataVOI[3] ||
		this->DataVOI[4] || this->DataVOI[5])
	{
		if ((this->DataVOI[0] < 0) ||
			(this->DataVOI[1] >= xsize) ||
			(this->DataVOI[2] < 0) ||
			(this->DataVOI[3] >= ysize))
		{
			vtkWarningMacro("The requested VOI is larger than the file's (" <<
				this->InternalFileName << ") extent ");
			this->DataVOI[0] = 0;
			this->DataVOI[1] = xsize - 1;
			this->DataVOI[2] = 0;
			this->DataVOI[3] = ysize - 1;
		}
	}

	this->DataExtent[0] = 0;
	this->DataExtent[1] = xsize - 1;
	this->DataExtent[2] = 0;
	this->DataExtent[3] = ysize - 1;

	this->SetDataScalarTypeToShort();
	this->SetNumberOfScalarComponents(1);
	this->SetDataByteOrderToLittleEndian();
	this->Superclass::ExecuteInformation();
}

void vtkIMGReader::ExecuteDataWithInformation(vtkDataObject* output, vtkInformation* outInfo)
{
	this->Superclass::ExecuteDataWithInformation(output, outInfo);

	if (this->UpdateExtentIsEmpty(outInfo, output))
	{
		return;
	}
	int xsize = this->DataExtent[1] + 1;
	int ysize = this->DataExtent[3] + 1;

	vtkImageData* data = vtkImageData::SafeDownCast(output);
	if (data)
	{
		short* buffer = static_cast<short*>(data->GetScalarPointer());
		if (!buffer) return;
		short* temp = static_cast<short*>(malloc(xsize * ysize * sizeof(short)));
		if (!temp) return;
		short* first = temp;
		for (int ix = 0; ix < xsize; ++ix)
		{
			for (int iy = 0; iy < ysize; ++iy)
			{
				temp[iy * xsize + ix] = *buffer++;
			}
		}

		memcpy(reinterpret_cast<void*>(data->GetScalarPointer()), reinterpret_cast<void*>(temp), xsize * ysize * sizeof(short));
		free(temp);
	}
}

//----------------------------------------------------------------------------
int vtkIMGReader::CanReadFile(const char* fname)
{
	FILE* fp = fopen(fname, "rb");
	if (!fp) return 0;

	struct stat statbuf;
	fstat(fileno(fp), &statbuf);

	char buff[8];
	fread(buff, 1, 8, fp);
	int xsize;
	int ysize;
	fread((void*)&xsize, 1, 4, fp);
	fread((void*)&ysize, 1, 4, fp);

	fclose(fp);

	int header = static_cast<int>(statbuf.st_size - xsize * ysize * 2);

	return (16 != header) ? 0 : 3;
}

//----------------------------------------------------------------------------
void vtkIMGReader::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
}
