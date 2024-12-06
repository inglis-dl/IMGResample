/*=========================================================================

  Module:    vtkIMGReader.h
  Program:
  Language:  C++
  Author:    Dean Inglis <inglisd AT gmail DOT com>

=========================================================================*/
// .NAME vtkIMGReader - read IMG files
// .SECTION Description
// vtkIMGReader is a source object that reads IMG files.
//

#ifndef __vtkIMGReader_h
#define __vtkIMGReader_h

#include "vtkImageReader.h"

class vtkIMGReader : public vtkImageReader
{
public:
	static vtkIMGReader* New();
	vtkTypeMacro(vtkIMGReader, vtkImageReader);
	virtual void PrintSelf(ostream& os, vtkIndent indent) override;

	int CanReadFile(VTK_FILEPATH const char*) override;

	// Description:
	// .img
	const char* GetFileExtensions() override { return ".img .msk"; }

	// Description:
	// IMG
	const char* GetDescriptiveName() override { return "IMG file"; }

protected:
	vtkIMGReader();
	~vtkIMGReader() override = default;

	void ExecuteInformation() override;
	void ExecuteDataWithInformation(vtkDataObject* out, vtkInformation* outInfo) override;

private:
	vtkIMGReader(const vtkIMGReader&) = delete;  // Not implemented.
	void operator=(const vtkIMGReader&) = delete;  // Not implemented.
};

#endif


