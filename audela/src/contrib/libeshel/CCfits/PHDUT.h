//1
//2
//3
//4

//	This is version 2.0 release dated Jan 2008

//	Astrophysics Science Division,
//	NASA/ Goddard Space Flight Center
//	HEASARC
//	http://heasarc.gsfc.nasa.gov
//	e-mail: ccfits@legacy.gsfc.nasa.gov
//
//	Original author: Ben Dorman


#ifndef PHDUT_H
#define PHDUT_H
#include "PrimaryHDU.h"
#include <iostream>
#include <exception>

namespace CCfits
{

        template <typename S>
        void PHDU::read (std::valarray<S>& image)
        {
                long init(1);
                long nElements(std::accumulate(naxes().begin(),naxes().end(),init,
                                std::multiplies<long>()));

                read(image,1,nElements,static_cast<S*>(0));
        }


        template <typename S>
        void  PHDU::read (std::valarray<S>& image, long first,long nElements) 
        {
                read(image, first,nElements,static_cast<S*>(0));
        }

        template <typename S>
        void  PHDU::read (std::valarray<S>& image, long first, long nElements,  S* nullValue) 
        {
                makeThisCurrent();
                if ( PrimaryHDU<S>* phdu = dynamic_cast<PrimaryHDU<S>*>(this) )
                {
                        // proceed if cast is successful.
                        const std::valarray<S>& __tmp = phdu->readImage(first,nElements,nullValue);                           
                        image.resize(__tmp.size());
                        image = __tmp;
                }
                else
                {
                        if (bitpix() == Ifloat)
                        {
                                PrimaryHDU<float>& phdu 
                                                = dynamic_cast<PrimaryHDU<float>&>(*this);
                                float nulVal(0);
                                if (nullValue) nulVal = static_cast<float>(*nullValue);                                 
                                FITSUtil::fill(image,phdu.readImage(first,nElements,&nulVal));

                        }
                        else if (bitpix() == Idouble)
                        {
                                PrimaryHDU<double>& phdu 
                                                = dynamic_cast<PrimaryHDU<double>&>(*this);
                                double nulVal(0);
                                if (nullValue) nulVal = static_cast<double>(*nullValue);                                 
                                FITSUtil::fill(image,phdu.readImage(first,nElements,&nulVal));

                        }
                        else if (bitpix() == Ibyte)
                        {
                                PrimaryHDU<unsigned char>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned char>&>(*this);
                                unsigned char nulVal(0);
                                if (nullValue) nulVal = static_cast<unsigned char>(*nullValue);                                 
                                FITSUtil::fill(image,phdu.readImage(first,nElements,&nulVal));                               
                        } 
                        else if (bitpix() == Ilong)
                        {
                                if ( zero() == ULBASE && scale() == 1)
                                {
                                        PrimaryHDU<unsigned long>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned long>&>(*this);
                                        unsigned long nulVal(0);
                                        if (nullValue) nulVal 
                                                = static_cast<unsigned long>(*nullValue);                                 
                                        FITSUtil::fill(image,
                                                        phdu.readImage(first,nElements,&nulVal));                              
                                }
                                else
                                {
                                        PrimaryHDU<long>& phdu 
                                                        = dynamic_cast<PrimaryHDU<long>&>(*this);
                                        long nulVal(0);
                                        if (nullValue) nulVal = static_cast<long>(*nullValue);                                 
                                        FITSUtil::fill(image,
                                                        phdu.readImage(first,nElements,&nulVal));                              
                                }
                        }    
                        else if (bitpix() == Ishort)
                        {
                                if ( zero() == USBASE && scale() == 1)
                                {
                                        PrimaryHDU<unsigned short>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned short>&>(*this);
                                        unsigned short nulVal(0);
                                        if (nullValue) nulVal 
                                                        = static_cast<unsigned short>(*nullValue);                                 
                                        FITSUtil::fill(image,
                                                        phdu.readImage(first,nElements,&nulVal));                             
                                }
                                else
                                {
                                        PrimaryHDU<short>& phdu 
                                                        = dynamic_cast<PrimaryHDU<short>&>(*this);
                                        short nulVal(0);
                                        if (nullValue) nulVal = static_cast<short>(*nullValue);                                 
                                        FITSUtil::fill(image,
                                                        phdu.readImage(first,nElements,&nulVal));                             

                                }
                        }          
                        else 
                        {
                                throw CCfits::FitsFatal(" casting image types ");
                        }     
                }

        }  

        template<typename S>
        void  PHDU::read (std::valarray<S>& image, const std::vector<long>& first, 
				long nElements, 
				S* nullValue)
        {
                makeThisCurrent();
                long firstElement(0);
                long dimSize(1);
                std::vector<long> inputDimensions(naxis(),1);
                size_t sNaxis = static_cast<size_t>(naxis());
                size_t n(std::min(sNaxis,first.size()));
                std::copy(&first[0],&first[n],&inputDimensions[0]);                
                for (long i = 0; i < naxis(); ++i)
                {

                   firstElement +=  ((inputDimensions[i] - 1)*dimSize);
                   dimSize *=naxes(i);   
                }
                ++firstElement;                


                read(image, firstElement,nElements,nullValue);



        } 

        template<typename S>
        void  PHDU::read (std::valarray<S>& image, const std::vector<long>& first, 
				long nElements)
        {
                read(image, first,nElements,static_cast<S*>(0));

        } 

        template<typename S>
        void  PHDU::read (std::valarray<S>& image, const std::vector<long>& firstVertex, 
				const std::vector<long>& lastVertex, 
				const std::vector<long>& stride, 
				S* nullValue)
        {
                makeThisCurrent();
                if (PrimaryHDU<S>* phdu = dynamic_cast<PrimaryHDU<S>*>(this))
                {
                        const std::valarray<S>& __tmp 
                                        = phdu->readImage(firstVertex,lastVertex,stride,nullValue);                         
                        image.resize(__tmp.size());
                        image = __tmp;
                }
                else
                {
                        // FITSutil::fill will take care of sizing.
                        if (bitpix() == Ifloat)
                        {
                                float nulVal(0);
                                if (nullValue) nulVal = static_cast<float>(*nullValue);                                 
                                PrimaryHDU<float>& phdu = dynamic_cast<PrimaryHDU<float>&>(*this);
                                FITSUtil::fill(image,
                                        phdu.readImage(firstVertex,lastVertex,stride,&nulVal));
                        }
                        else if (bitpix() == Idouble)
                        {
                                PrimaryHDU<double>& phdu = dynamic_cast<PrimaryHDU<double>&>(*this);
                                double nulVal(0);
                                if (nullValue) nulVal = static_cast<double>(*nullValue);                                 
                                FITSUtil::fill(image,
                                                phdu.readImage(firstVertex,lastVertex,stride,&nulVal));                              
                        }
                        else if (bitpix() == Ibyte)
                        {
                                PrimaryHDU<unsigned char>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned char>&>(*this);
                                unsigned char nulVal(0);
                                if (nullValue) nulVal = static_cast<unsigned char>(*nullValue);                                 
                                FITSUtil::fill(image,
                                                phdu.readImage(firstVertex,lastVertex,stride,&nulVal));
                        } 
                        else if (bitpix() == Ilong)
                        {
                                if ( zero() == ULBASE && scale() == 1)
                                {
                                        PrimaryHDU<unsigned long>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned long>&>(*this);
                                        unsigned long nulVal(0);
                                        if (nullValue) nulVal 
                                                = static_cast<unsigned long>(*nullValue);                                 
                                        FITSUtil::fill(image,
                                             phdu.readImage(firstVertex,lastVertex,stride,&nulVal));                            
                                }
                                else
                                {
                                        PrimaryHDU<long>& phdu 
                                                        = dynamic_cast<PrimaryHDU<long>&>(*this);
                                        long nulVal(0);
                                        if (nullValue) nulVal = static_cast<long>(*nullValue);                                 
                                        FITSUtil::fill(image,
                                             phdu.readImage(firstVertex,lastVertex,stride,&nulVal));                            
                                }      
                        }    
                        else if (bitpix() == Ishort)
                        {
                                if ( zero() == USBASE && scale() == 1)
                                {
                                        PrimaryHDU<unsigned short>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned short>&>(*this);
                                        unsigned short nulVal(0);
                                        if (nullValue) nulVal 
                                                = static_cast<unsigned short>(*nullValue);                                 
                                        FITSUtil::fill(image,
                                             phdu.readImage(firstVertex,lastVertex,stride,&nulVal));                            
                                }
                                else
                                {
                                        PrimaryHDU<short>& phdu 
                                                        = dynamic_cast<PrimaryHDU<short>&>(*this);
                                        short nulVal(0);
                                        if (nullValue) nulVal = static_cast<short>(*nullValue);                                 
                                        FITSUtil::fill(image,
                                             phdu.readImage(firstVertex,lastVertex,stride,&nulVal));                            
                                }
                        }          
                        else 
                        {
                                throw CCfits::FitsFatal(" casting image types ");
                        }     
                }
        }  

        template<typename S>
        void  PHDU::read (std::valarray<S>& image, const std::vector<long>& firstVertex, 
				const std::vector<long>& lastVertex, 
				const std::vector<long>& stride)
        {
                read(image, firstVertex,lastVertex,stride,static_cast<S*>(0));
        }  

        template <typename S>
        void PHDU::write(long first,
                            long nElements,
                        const std::valarray<S>& data,
                        S* nullValue)
        {                

                // for integer types, must do the null value substitution before
                // data conversion.
                makeThisCurrent();
                if (PrimaryHDU<S>* image = dynamic_cast<PrimaryHDU<S>*>(this))
                {
                        image->writeImage(first,nElements,data,nullValue);
                }
                else
                {
                        if (bitpix() == Ifloat)
                        {
                                std::valarray<float> __tmp;                               
                                PrimaryHDU<float>& phdu = dynamic_cast<PrimaryHDU<float>&>(*this);
                                FITSUtil::fill(__tmp,data);
                                phdu.writeImage(first,nElements,__tmp,static_cast<float*>(nullValue));
                        }
                        else if (bitpix() == Idouble)
                        {
                                std::valarray<double> __tmp;                                
                                PrimaryHDU<double>& phdu 
                                                = dynamic_cast<PrimaryHDU<double>&>(*this);
                                FITSUtil::fill(__tmp,data);
                                phdu.writeImage(first,nElements,__tmp,
                                                static_cast<double*>(nullValue));                              
                        }
                        else if (bitpix() == Ibyte)
                        {
                                PrimaryHDU<unsigned char>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned char>&>(*this);
                                std::valarray<unsigned char> __tmp;         
                                unsigned char blankVal(0);      
                                try 
                                {
                                        readKey("BLANK",blankVal);
                                        std::valarray<S> copyData(data);
                                        std::replace(&copyData[0],&copyData[data.size()],
                                                static_cast<unsigned char>(*nullValue),blankVal);

                                        FITSUtil::fill(__tmp,copyData);                                        

                                        phdu.writeImage(first,nElements,__tmp);                          }
                                catch (HDU::NoSuchKeyword)
                                {
                                        throw NoNullValue("Primary");
                                }

                        } 
                        else if (bitpix() == Ilong)
                        {                               
                                if ( zero() == ULBASE && scale() == 1)
                                {
                                        PrimaryHDU<unsigned long>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned long>&>(*this);
                                        std::valarray<unsigned long> __tmp;   
                                        unsigned long blankVal(0);                              
                                        try 
                                        {
                                                readKey("BLANK",blankVal);
                                                std::valarray<S> copyData(data);
                                                std::replace(&copyData[0],&copyData[data.size()],
                                                   static_cast<unsigned long>(*nullValue),blankVal);
                                                FITSUtil::fill(__tmp,copyData);                                        
                                                phdu.writeImage(first,nElements,__tmp);                          
                                        }
                                        catch (HDU::NoSuchKeyword)
                                        {
                                                throw NoNullValue("Primary");
                                        }                        
                                }
                                else
                                {
                                        PrimaryHDU<long>& phdu 
                                                        = dynamic_cast<PrimaryHDU<long>&>(*this);
                                        std::valarray<long> __tmp;   
                                        long blankVal(0);                              
                                        try 
                                        {
                                                readKey("BLANK",blankVal);
                                                std::valarray<S> copyData(data);
                                                std::replace(&copyData[0],&copyData[data.size()],
                                                     static_cast<long>(*nullValue),blankVal);

                                                FITSUtil::fill(__tmp,copyData);                                        
                                                phdu.writeImage(first,nElements,__tmp);                          
                                        }
                                        catch (HDU::NoSuchKeyword)
                                        {
                                                throw NoNullValue("Primary");
                                        }                        
                                }
                        }    
                        else if (bitpix() == Ishort)
                        {
                                if ( zero() == USBASE && scale() == 1)
                                {
                                        PrimaryHDU<unsigned short>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned short>&>(*this);
                                        std::valarray<unsigned short> __tmp;   
                                        unsigned short blankVal(0);                              
                                        try 
                                        {
                                                readKey("BLANK",blankVal);
                                                std::valarray<S> copyData(data);
                                                std::replace(&copyData[0],&copyData[data.size()],
                                                   static_cast<unsigned short>(*nullValue),blankVal);
                                                FITSUtil::fill(__tmp,copyData);                                        
                                                phdu.writeImage(first,nElements,__tmp);                          
                                        }
                                        catch (HDU::NoSuchKeyword)
                                        {
                                                throw NoNullValue("Primary");
                                        }                        
                                }
                                else
                                {
                                        PrimaryHDU<short>& phdu 
                                                        = dynamic_cast<PrimaryHDU<short>&>(*this);
                                        std::valarray<short> __tmp;   
                                        short blankVal(0);                              
                                        try 
                                        {
                                                readKey("BLANK",blankVal);
                                                std::valarray<S> copyData(data);
                                                std::replace(&copyData[0],&copyData[data.size()],
                                                     static_cast<short>(*nullValue),blankVal);

                                                FITSUtil::fill(__tmp,copyData);                                        
                                                phdu.writeImage(first,nElements,__tmp);                          
                                        }
                                        catch (HDU::NoSuchKeyword)
                                        {
                                                throw NoNullValue("Primary");
                                        }                        
                                }
                        }           
                        else
                        {
                                FITSUtil::MatchType<S> errType;                                
                                throw FITSUtil::UnrecognizedType(FITSUtil::FITSType2String(errType()));
                        }        
                }
        }


        template <typename S>
        void PHDU::write(long first,
                            long nElements,
                        const std::valarray<S>& data)
        {                

                // for integer types, must do the null value substitution before
                // data conversion.
                makeThisCurrent();
                if ( PrimaryHDU<S>* image = dynamic_cast<PrimaryHDU<S>*>(this) )
                {
                        image->writeImage(first,nElements,data);
                }
                else
                {
                        if (bitpix() == Ifloat)
                        {
                                std::valarray<float> __tmp;                               
                                PrimaryHDU<float>& phdu = dynamic_cast<PrimaryHDU<float>&>(*this);
                                FITSUtil::fill(__tmp,data);
                                phdu.writeImage(first,nElements,__tmp);
                        }
                        else if (bitpix() == Idouble)
                        {
                                std::valarray<double> __tmp;                                
                                PrimaryHDU<double>& phdu 
                                                = dynamic_cast<PrimaryHDU<double>&>(*this);
                                FITSUtil::fill(__tmp,data);
                                phdu.writeImage(first,nElements,__tmp);                              
                        }
                        else if (bitpix() == Ibyte)
                        {
                                PrimaryHDU<unsigned char>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned char>&>(*this);
                                std::valarray<unsigned char> __tmp;         
                                FITSUtil::fill(__tmp,data);                                        
                                phdu.writeImage(first,nElements,__tmp);
                        } 
                        else if (bitpix() == Ilong)
                        {                               
                                if ( zero() == ULBASE && scale() == 1)
                                {
                                        PrimaryHDU<unsigned long>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned long>&>(*this);
                                        std::valarray<unsigned long> __tmp;   
                                        FITSUtil::fill(__tmp,data);                                        
                                        phdu.writeImage(first,nElements,__tmp);                          
                                }
                                else
                                {
                                        PrimaryHDU<long>& phdu 
                                                        = dynamic_cast<PrimaryHDU<long>&>(*this);
                                        std::valarray<long> __tmp;   
                                        FITSUtil::fill(__tmp,data);                                        
                                        phdu.writeImage(first,nElements,__tmp);                          
                                }
                        }    
                        else if (bitpix() == Ishort)
                        {
                                if ( zero() == USBASE && scale() == 1)
                                {
                                        PrimaryHDU<unsigned short>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned short>&>(*this);
                                        std::valarray<unsigned short> __tmp;   
                                        FITSUtil::fill(__tmp,data);                                        
                                        phdu.writeImage(first,nElements,__tmp);
                                }
                                else
                                {
                                        PrimaryHDU<short>& phdu 
                                                = dynamic_cast<PrimaryHDU<short>&>(*this);
                                        std::valarray<short> __tmp;   
                                        FITSUtil::fill(__tmp,data);                                        
                                        phdu.writeImage(first,nElements,__tmp);                                        
                                }                          
                        }
                        else
                        {
                                FITSUtil::MatchType<S> errType;                                
                                throw FITSUtil::UnrecognizedType(FITSUtil::FITSType2String(errType()));
                        }        
                }
        }        

        template <typename S>
        void PHDU::write(const std::vector<long>& first,
                        long nElements,
                        const std::valarray<S>& data,
                        S* nullValue)
        {        
                makeThisCurrent();
                size_t n(first.size());
                long firstElement(0);
                long dimSize(1);
                for (long i = 0; i < first.size(); ++i)
                {
                        firstElement +=  ((first[i] - 1)*dimSize);
                        dimSize *=naxes(i);   
                }       
                ++firstElement;

                write(firstElement,nElements,data,nullValue);
        }

        template <typename S>
        void PHDU::write(const std::vector<long>& first,
                        long nElements,
                        const std::valarray<S>& data)
        {        
                makeThisCurrent();
                size_t n(first.size());
                long firstElement(0);
                long dimSize(1);
                for (long i = 0; i < first.size(); ++i)
                {

                        firstElement +=  ((first[i] - 1)*dimSize);
                        dimSize *=naxes(i);   
                }       
                ++firstElement;

                write(firstElement,nElements,data);                     
        }        


        template <typename S>
        void PHDU::write(const std::vector<long>& firstVertex,
                        const std::vector<long>& lastVertex,
			const std::vector<long>& stride, 
                        const std::valarray<S>& data)
        {
                makeThisCurrent();
                try
                {
                        PrimaryHDU<S>& image = dynamic_cast<PrimaryHDU<S>&>(*this);
                        image.writeImage(firstVertex,lastVertex,stride,data);  
                }
                catch (std::bad_cast)
                {
                         // write input type S to Image type...

                        if (bitpix() == Ifloat)
                        {
                                PrimaryHDU<float>& phdu = dynamic_cast<PrimaryHDU<float>&>(*this);
                                size_t n(data.size());
                                std::valarray<float> __tmp(n);
                                for (size_t j= 0; j < n; ++j) __tmp[j] = data[j];
                                phdu.writeImage(firstVertex,lastVertex,stride,__tmp);

                        }
                        else if (bitpix() == Idouble)
                        {
                                PrimaryHDU<double>& phdu 
                                        = dynamic_cast<PrimaryHDU<double>&>(*this);
                                size_t n(data.size());
                                std::valarray<double> __tmp(n);
                                for (size_t j= 0; j < n; ++j) __tmp[j] = data[j];
                                phdu.writeImage(firstVertex,lastVertex,stride,__tmp);
                        }
                        else if (bitpix() == Ibyte)
                        {
                                PrimaryHDU<unsigned char>& phdu 
                                        = dynamic_cast<PrimaryHDU<unsigned char>&>(*this);
                                size_t n(data.size());
                                std::valarray<unsigned char> __tmp(n);
                                for (size_t j= 0; j < n; ++j) __tmp[j] = data[j];
                                phdu.writeImage(firstVertex,lastVertex,stride,__tmp);                        
                        } 
                        else if (bitpix() == Ilong)
                        {
                                if ( zero() == ULBASE && scale() == 1)
                                {
                                        PrimaryHDU<unsigned long>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned long>&>(*this);
                                        size_t n(data.size());
                                        std::valarray<unsigned long> __tmp(n);
                                        for (size_t j= 0; j < n; ++j) __tmp[j] = data[j];
                                        phdu.writeImage(firstVertex,lastVertex,stride,__tmp);                         

                                }
                                else
                                {
                                        PrimaryHDU<long>& phdu 
                                                        = dynamic_cast<PrimaryHDU<long>&>(*this);
                                        size_t n(data.size());
                                        std::valarray<long> __tmp(n);
                                        for (size_t j= 0; j < n; ++j) __tmp[j] = data[j];
                                        phdu.writeImage(firstVertex,lastVertex,stride,__tmp);                         
                                }
                        }                           
                        else if (bitpix() == Ishort)
                        {
                                if ( zero() == USBASE && scale() == 1)
                                {
                                        PrimaryHDU<unsigned short>& phdu 
                                                = dynamic_cast<PrimaryHDU<unsigned short>&>(*this);
                                        size_t n(data.size());
                                        std::valarray<unsigned short> __tmp(n);
                                        for (size_t j= 0; j < n; ++j) __tmp[j] = data[j];
                                        phdu.writeImage(firstVertex,lastVertex,stride,__tmp);                         

                                }
                                else
                                {
                                        PrimaryHDU<short>& phdu 
                                                        = dynamic_cast<PrimaryHDU<short>&>(*this);
                                        size_t n(data.size());
                                        std::valarray<short> __tmp(n);
                                        for (size_t j= 0; j < n; ++j) __tmp[j] = data[j];
                                        phdu.writeImage(firstVertex,lastVertex,stride,__tmp);                         
                                }
                        }          
                        else
                        {
                                FITSUtil::MatchType<S> errType;                                
                                throw FITSUtil::UnrecognizedType(FITSUtil::FITSType2String(errType()));
                        }        
                }  
        }  




} // namespace CCfits
#endif
