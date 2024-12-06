/*=========================================================================

  Module:    vtkIMGWriter.cxx
  Program:
  Language:  C++
  Author:    Dean Inglis <inglisd AT gmail DOT com>

=========================================================================*/
#include <vtkIMGWriter.h>

// VTK includes
#include <vtkErrorCode.h>
#include <vtkImageData.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>

vtkStandardNewMacro(vtkIMGWriter);

//----------------------------------------------------------------------------
void vtkIMGWriter::WriteFileHeader(ostream* file, vtkImageData* cache, int wExt[6])
{
	int min1 = wExt[0], max1 = wExt[1], min2 = wExt[2], max2 = wExt[3];
	const int bpp = cache->GetNumberOfScalarComponents();

	if (1 != bpp)
	{
		vtkErrorMacro(<< "IMG Writer only supports 1 scalar component");
		return;
	}

	int imgType = 99; // PC image type
	file->write((char*)&imgType, sizeof(int));
	int dummy = 0;
	file->write((char*)&dummy, sizeof(int));
	int width = (max1 - min1 + 1);
	int height = (max2 - min2 + 1);
	file->write((char*)&width, sizeof(int));
	file->write((char*)&height, sizeof(int));
}

//----------------------------------------------------------------------------
void vtkIMGWriter::WriteFile(ostream* file, vtkImageData* data,
	int extent[6], int wExt[6])
{
	int idx0, idx1, idx2;
	int rowLength; // in bytes
	void* ptr;
	unsigned long count = 0;
	unsigned long target;
	float progress = this->Progress;
	float area;

	// Make sure we actually have data.
	if (!data->GetPointData()->GetScalars())
	{
		vtkErrorMacro(<< "Could not get data from input.");
		return;
	}

	// take into consideration the scalar type
	switch (data->GetScalarType())
	{
	case VTK_UNSIGNED_SHORT:
		rowLength = sizeof(unsigned short);
		break;
	case VTK_SHORT:
		rowLength = sizeof(short);
		break;
	default:
		vtkErrorMacro("IMGWriter only accepts unsigned short scalars!");
		return;
	}
	rowLength *= data->GetNumberOfScalarComponents();

	area = static_cast<float>(((extent[5] - extent[4] + 1) * (extent[3] - extent[2] + 1) *
		(extent[1] - extent[0] + 1))) /
		static_cast<float>(((wExt[5] - wExt[4] + 1) * (wExt[3] - wExt[2] + 1) *
			(wExt[1] - wExt[0] + 1)));

	target = (unsigned long)((extent[5] - extent[4] + 1) *
		(extent[3] - extent[2] + 1) / (50.0 * area));
	target++;

	for (idx2 = extent[4]; idx2 <= extent[5]; ++idx2)
	{
		for (idx1 = extent[0]; idx1 <= extent[1]; idx1++)
		{
			if (!(count % target))
			{
				this->UpdateProgress(progress + count / (50.0 * target));
			}
			count++;
			for (idx0 = extent[2]; idx0 <= extent[3]; idx0++)
			{
				ptr = data->GetScalarPointer(idx1, idx0, idx2);
				if (!file->write(static_cast<char*>(ptr), rowLength))
				{
					this->SetErrorCode(vtkErrorCode::OutOfDiskSpaceError);
					return;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
void vtkIMGWriter::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
}
