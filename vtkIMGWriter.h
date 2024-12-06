/*=========================================================================

  Module:    vtkIMGWriter.h
  Program:
  Language:  C++
  Author:    Dean Inglis <inglisd AT gmail DOT com>

=========================================================================*/
// .NAME vtkIMGWriter - Writes IMG files.
// .SECTION Description
// vtkIMGWriter writes IMG files. It supports 1 component data of
// unsigned short.

// .SECTION See Also
// vtkIMGReader

#ifndef __vtkIMGWriter_h
#define __vtkIMGWriter_h

#include <vtkImageWriter.h>

class vtkIMGWriter : public vtkImageWriter
{
public:
	static vtkIMGWriter* New();
	vtkTypeMacro(vtkIMGWriter, vtkImageWriter);
	void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
	vtkIMGWriter() = default;
	~vtkIMGWriter() override = default;

	void WriteFile(ostream* file, vtkImageData* data, int extent[6], int wExt[6]) override;
	void WriteFileHeader(ostream*, vtkImageData*, int wExt[6]) override;

private:
	vtkIMGWriter(const vtkIMGWriter&) = delete;  // Not implemented.
	void operator=(const vtkIMGWriter&) = delete;  // Not implemented.
};

#endif
