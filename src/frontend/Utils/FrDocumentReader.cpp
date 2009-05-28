#include "FrDocumentReader.h"
#include "FrDocument.h"
#include "FrDocumentObj.h"
#include "FrImageDocObj.h"
#include "FrRoiDocObj.h"
#include "FrUtils.h"
#include "FrPointsDocObj.h"

#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"

// Backend includes
#include "RtMRIImage.h"
#include "RtMaskImage.h"
#include "RtActivation.h"
#include "RtExperiment.h"

vtkStandardNewMacro(FrDocumentReader);


FrDocumentReader::FrDocumentReader()
: m_Document(0), m_Mosaic(false), 
  m_Target(FrDocumentReader::Mri),
  m_Orientation(FrDocumentReader::XY), 
  m_Slice(0), m_DataID(0), m_Output(0) {
}

FrDocumentReader::~FrDocumentReader(){
    this->SetOutput(0);
}

void FrDocumentReader::Update(){
    if(!m_Document) return;

    vtkImageData* result = 0;
    switch(m_Target){
        case Mri:
            result = this->ReadMri();
            break;
        case Roi:
            result = this->ReadRoi();
            break;
        case Activation:
            result = this->ReadActivation();
            break;
        case Points:
            result = this->ReadPoints();
            break;
    }

    this->SetOutput(result);
}

void FrDocumentReader::SetMosaic(bool value){
    if(m_Mosaic != value){
        m_Mosaic = value;
        this->SetOutput(0);
    }
}
    
void FrDocumentReader::SetDocument(FrDocument* document){
    if(m_Document != document){
        m_Document = document;
        this->SetOutput(0);
    }
}

void FrDocumentReader::SetTarget(Targets target){
    if(m_Target != target){
        m_Target = target;
        this->SetOutput(0);
    }
}

void FrDocumentReader::SetDataID(const RtDataID &id){
  //if(m_DataID != id){
        m_DataID = id;
        this->SetOutput(0);
	//}
}

//void FrDocumentReader::SetTimePoint(unsigned int timePoint){
//  if(m_DataID.getTimePoint() != timePoint){
//    m_DataID.setTimePoint(timePoint);
//    this->SetOutput(0);
//  }
//}
 
void FrDocumentReader::SetOrientation(Orientations value){
    if(m_Orientation != value){
        m_Orientation = value;
        this->SetOutput(0);
    }
}

void FrDocumentReader::SetSlice(int slice){
    if(m_Slice != slice){
        m_Slice = slice;
        this->SetOutput(0);
    }
}
    
void FrDocumentReader::SetOutput(vtkImageData* data){
    if(data){
        data->Register(this);
        data->Delete();
    }
    if(m_Output){
        m_Output->UnRegister(this);
    }
    m_Output = data;
}

// Helpers
vtkImageData* FrDocumentReader::ReadMri(){
    // ohinds 2009-02-28
    // changed to get all images from data store

  //getDataStore().getAvailableData();  

    // Find appropriate image volume
  RtMRIImage* mri = dynamic_cast<RtMRIImage*>(getDataStore().getData(m_DataID));

    //FrDocument::DocObjCollection images;
    //m_Document->GetObjectsByType(images, FrDocumentObj::ImageObject);

    

//    // NOTE Since we support the only one series 
//    // Time point is a unique data ID.
//    int timePoint = m_DataID;
//    if(images.size() > 0){
//        FrImageDocObj* imgDO = (FrImageDocObj*)images[0];
//        mri = imgDO->GetTimePointData(timePoint);
//    }
    
    // Process data
    vtkImageData* result = 0L;
    if(mri){
        result = this->GetMriSlice(mri);
    }

    return result;
} 

vtkImageData* FrDocumentReader::ReadRoi(){
    // ohinds 2009-02-28
    // changed to get all images from data store

    // Find appropriate image volume
  RtMaskImage* mask = dynamic_cast<RtMaskImage*>(getDataStore().getData(m_DataID));

//    FrDocument::DocObjCollection rois;
//    m_Document->GetObjectsByType(rois, FrDocumentObj::RoiObject);
//
//    int roiID = m_DataID;
//    FrDocument::DocObjCollection::iterator it, itEnd(rois.end());
//    for(it = rois.begin(); it != itEnd; ++it){
//        FrRoiDocObj* roiDO = (FrRoiDocObj*)(*it);
//        if(roiDO->GetID() == roiID){
//            mask = roiDO->GetMaskImage();
//            break;
//        }
//    }

    // 
    vtkImageData* result = 0L;
    if(mask){
        result = this->GetRoiSlice(mask);
    }

    return result;
}

vtkImageData* FrDocumentReader::ReadActivation(){
    // TODO: implement
    // Not supported for a while
    return 0L;
}

vtkImageData* FrDocumentReader::ReadPoints(){
    vtkImageData* result = 0L;

    // don't show points if we have no any image opened
    FrDocument::DocObjCollection imageObjects;
    m_Document->GetObjectsByType(imageObjects, FrDocumentObj::ImageObject);
    if ( imageObjects.size() == 0)
        return 0L;

    FrPointsDocObj* pointsDO = 0L;
    FrDocument::DocObjCollection pointObjects;
    m_Document->GetObjectsByType(pointObjects, FrDocumentObj::PointsObject);    
    
    if(pointObjects.size() > 0){
        pointsDO = (FrPointsDocObj*)pointObjects[0];

        // Find appropriate image volume
	RtMRIImage* mri = dynamic_cast<RtMRIImage*>(getDataStore().getData(m_DataID));
//        RtMRIImage* mri = 0;
//        FrDocument::DocObjCollection images;
//        m_Document->GetObjectsByType(images, FrDocumentObj::ImageObject);
//
//        // NOTE Since we support the only one series 
//        // Time point is a unique data ID.
//        int timePoint = m_DataID;
//
//        if(images.size() > 0){
//            FrImageDocObj* imgDO = (FrImageDocObj*)images[0];
//            mri = imgDO->GetTimePointData(timePoint);
//        }
        pointsDO->GetDimsFromImg(mri);

        if (m_Mosaic){
            RtMRIImage* image = new RtMRIImage(*mri);
        
            if(!image->mosaic()){
                delete image;
                return 0L;
            }
            
            int xDim = image->getDim(0);
            int yDim = image->getDim(1);

            result = pointsDO->GetMosaicData(xDim, yDim);
        }
        else{
            switch(m_Orientation){
                case XY:
                    result = pointsDO->GetPointsXY(m_Slice);
                    break;
                case XZ:
                    result = pointsDO->GetPointsXZ(m_Slice);
                    break;
                case YZ:
                    result = pointsDO->GetPointsYZ(m_Slice);
                    break;
            }
        }
        return result;
    }
    
    return 0L;
}

CrosshairParams FrDocumentReader::ReadCrosshair(){
  CrosshairParams result;

  // don't show points if we have no any image opened
  FrDocument::DocObjCollection imageObjects;
  m_Document->GetObjectsByType(imageObjects, FrDocumentObj::ImageObject);
  if ( imageObjects.size() == 0)
    return result;

  FrPointsDocObj* pointsDO = 0L;
  FrDocument::DocObjCollection pointObjects;
  m_Document->GetObjectsByType(pointObjects, FrDocumentObj::PointsObject);    
    
  if(pointObjects.size() > 0){
    pointsDO = (FrPointsDocObj*)pointObjects[0];

    // Find appropriate image volume
    RtMRIImage* mri = dynamic_cast<RtMRIImage*>(getDataStore().getData(m_DataID));

    if(mri == NULL) {
      return result;
    }
//    RtMRIImage* mri = 0;
//    FrDocument::DocObjCollection images;
//    m_Document->GetObjectsByType(images, FrDocumentObj::ImageObject);
//
//    // NOTE Since we support the only one series 
//    // Time point is a unique data ID.
//    int timePoint = m_DataID;
//
//    if(images.size() > 0){
//      FrImageDocObj* imgDO = (FrImageDocObj*)images[0];
//      mri = imgDO->GetTimePointData(timePoint);
//    }
    pointsDO->GetDimsFromImg(mri);

    if(m_Mosaic){
      unsigned int xDim = mri->getMosaicedWidth();
      unsigned int yDim = mri->getMosaicedHeight();
      result = pointsDO->GetCrosshairParamsMosaic(xDim, yDim);
    }
    else{
      switch(m_Orientation){
      case XY:
	result = pointsDO->GetCrosshairParams(XY);
	break;
      case XZ:
	result = pointsDO->GetCrosshairParams(XZ);
	break;
      case YZ:
	result = pointsDO->GetCrosshairParams(YZ);
	break;
      }
    }

  }
  
  return result;
}

vtkImageData* FrDocumentReader::GetMriSlice(RtMRIImage* mri){
    // Params
    RtMRIImage* image = 0;
    bool deleteImage = false;
    
    int xDim = 0;
    int yDim = 0;
    double pixDim0 = 1.0;
    double pixDim1 = 1.0;
    unsigned int dataSize = 0;
    short* pImageData = 0;
    
    // Assume that mri is always unmosaic
    if(m_Mosaic){
        deleteImage = true;
        image = new RtMRIImage(*mri);
                
        if(!image->mosaic()){
            delete image;
            return 0L;
        }
        
        xDim = image->getDim(0);
        yDim = image->getDim(1);
        pixDim0 = image->getPixDim(0);
        pixDim1 = image->getPixDim(1);
        dataSize = image->getNumPix();
        pImageData = image->getDataCopy();
    }
    else {
        deleteImage = false;
        image = mri;
                
        switch(m_Orientation){
            case XY:
                xDim = image->getDim(0);
                yDim = image->getDim(1);
                pixDim0 = image->getPixDim(0);
                pixDim1 = image->getPixDim(1);
                dataSize = xDim * yDim;
                // NOTE: here we clamp slice values
                m_Slice = ClampValue(m_Slice, 0, image->getDim(2) - 1);
                pImageData = this->GetSliceDataXY<short>(image, m_Slice);
                break;
            case YZ:
                xDim = image->getDim(1);
                yDim = image->getDim(2);
                pixDim0 = image->getPixDim(1);
                pixDim1 = image->getPixDim(2);
                dataSize = xDim * yDim;
                // NOTE: here we clamp slice values
                m_Slice = ClampValue(m_Slice, 0, image->getDim(0) - 1);
                pImageData = this->GetSliceDataYZ<short>(image, m_Slice);
                break;
            case XZ:
                xDim = image->getDim(0);
                yDim = image->getDim(2);
                pixDim0 = image->getPixDim(0);
                pixDim1 = image->getPixDim(2);
                dataSize = xDim * yDim;
                // NOTE: here we clamp slice values
                m_Slice = ClampValue(m_Slice, 0, image->getDim(1) - 1);
                pImageData = this->GetSliceDataXZ<short>(image, m_Slice);
                break;
        }
    }

    vnl_matrix_fixed<double, 4, 4> vxl2ras = image->getVxl2Ras();
   
    // center should be located in 0, 0, 0
    // find center of image
    double xcenter = xDim*pixDim0 / 2.0f;
    double ycenter = yDim*pixDim1 / 2.0f;

    // get shift value
    double x0 = -xcenter;
    double y0 = -ycenter;
    double z0 = 0.0f;
    
    //double x0 = vxl2ras.get(0, 3);
    //double y0 = vxl2ras.get(1, 3);
    //double z0 = vxl2ras.get(2, 3);

    vtkImageData* result = vtkImageData::New();
    result->SetScalarTypeToUnsignedChar();
    result->SetDimensions(xDim, yDim, 1);
    result->SetSpacing(pixDim0, pixDim1, 1.0);
    result->SetNumberOfScalarComponents(1);
    result->SetOrigin(x0, y0, z0);
    result->AllocateScalars();
    
    // Copy data and map values through lookup table
    unsigned char* LUT = 0;
    this->CreateMriLUT(mri->data, mri->getNumPix(), &LUT);

    short* pSrc = pImageData;
    unsigned char* pDst = (unsigned char*)result->GetScalarPointer();
    for(int i=0; i < dataSize; ++i){
        (*pDst) = LUT[(*pSrc)];
        ++pDst; ++pSrc;
    }

    // Cleanup
    delete[] LUT;
    delete[] pImageData;
    if(deleteImage){
        delete image;
    }
    return result;
} 

vtkImageData* FrDocumentReader::GetRoiSlice(RtMaskImage* roi){
    // Params
    RtMaskImage* mask = 0;
    bool deleteMask = false;
    
    int xDim = 0;
    int yDim = 0;
    double pixDim0 = 1.0;
    double pixDim1 = 1.0;

    unsigned int dataSize = 0;
    short* pMaskData = 0;
    
    // Assume that mri is always unmosaic
    if(m_Mosaic){
        deleteMask = true;
        mask = new RtMaskImage(*roi);
                
        if(!mask->mosaic()){
            delete mask;
            return 0L;
        }
        
        xDim = mask->getDim(0);
        yDim = mask->getDim(1);
        pixDim0 = mask->getPixDim(0);
        pixDim1 = mask->getPixDim(1);
        dataSize = mask->getNumPix();
        pMaskData = mask->getDataCopy();
    }
    else {
        deleteMask = false;
        mask = roi;

        switch(m_Orientation){
            case XY:
                xDim = mask->getDim(0);
                yDim = mask->getDim(1);
                pixDim0 = mask->getPixDim(0);
                pixDim1 = mask->getPixDim(1);
                dataSize = xDim * yDim;
                // NOTE: here we clamp slice values
                m_Slice = ClampValue(m_Slice, 0, mask->getDim(2)-1);
                pMaskData = this->GetSliceDataXY<short>(mask, m_Slice);
                break;
            case YZ:
                xDim = mask->getDim(1);
                yDim = mask->getDim(2);
                pixDim0 = mask->getPixDim(1);
                pixDim1 = mask->getPixDim(2);
                dataSize = xDim * yDim;
                // NOTE: here we clamp slice values
                m_Slice = ClampValue(m_Slice, 0, mask->getDim(0)-1);
                pMaskData = this->GetSliceDataYZ<short>(mask, m_Slice);
                break;
            case XZ:
                xDim = mask->getDim(0);
                yDim = mask->getDim(2);
                pixDim0 = mask->getPixDim(0);
                pixDim1 = mask->getPixDim(2);
                dataSize = xDim * yDim;
                // NOTE: here we clamp slice values
                m_Slice = ClampValue(m_Slice, 0, mask->getDim(1)-1);
                pMaskData = this->GetSliceDataXZ<short>(mask, m_Slice);
                break;
        }
    }

    vtkImageData* result = vtkImageData::New();
    result->SetScalarTypeToUnsignedChar();
    result->SetDimensions(xDim, yDim, 1);
    result->SetSpacing(pixDim0, pixDim1, 1.0);
    result->SetNumberOfScalarComponents(1);
    result->AllocateScalars();
    
    // Copy data 
    short* pSrc = pMaskData;
    unsigned char* pDst = (unsigned char*)result->GetScalarPointer();
    for(int i=0; i < dataSize; ++i){
        (*pDst) = (unsigned char)(*pSrc);
        ++pDst; ++pSrc;
    }

    // Cleanup
    delete[] pMaskData;
    if(deleteMask){
        delete mask;
    }
    return result;
}

void FrDocumentReader::CreateMriLUT(short* data, unsigned int dataSize, 
                                  unsigned char** outLUT){
    // assume that values cannot be negative
    short min = 0;
    short max = data[0]; 

    // find max and min values
    short* ptr = data;

    for(int i=0; i < dataSize; ++i){
        if(ptr[i] > max) max = ptr[i];
    }

    // create lut     
    (*outLUT) = new unsigned char[VTK_SHORT_MAX];
    memset((*outLUT), 0, VTK_SHORT_MAX);

    float mult = 255.0f / float(max - min);
    for(int i=min; i < max; ++i){
        (*outLUT)[i] = static_cast<unsigned char>((i - min) * mult);
    }
}
