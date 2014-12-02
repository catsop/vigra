/************************************************************************/
/*                                                                      */
/*                 Copyright 2011 by Ullrich Koethe                     */
/*                                                                      */
/*    This file is part of the VIGRA computer vision library.           */
/*    The VIGRA Website is                                              */
/*        http://hci.iwr.uni-heidelberg.de/vigra/                       */
/*    Please direct questions, bug reports, and contributions to        */
/*        ullrich.koethe@iwr.uni-heidelberg.de    or                    */
/*        vigra@informatik.uni-hamburg.de                               */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */
/*                                                                      */
/************************************************************************/

#define PY_ARRAY_UNIQUE_SYMBOL vigranumpyblockwise_PyArray_API
//#define NO_IMPORT_ARRAY

#include <vigra/numpy_array.hxx>
#include <vigra/numpy_array_converters.hxx>

#include <vigra/multi_blocking.hxx>
#include <vigra/multi_blockwise.hxx>

namespace python = boost::python;




namespace vigra{

    template<unsigned int DIM, class T_IN, class T_OUT>
    NumpyAnyArray pyBlockwiseGaussianSmoothMultiArray(
        const NumpyArray<DIM, T_IN> &  source, 
        const blockwise::BlockwiseConvolutionOptions<DIM>  & opt,
        NumpyArray<DIM, T_IN>  dest
    ){
        dest.reshapeIfEmpty(source.taggedShape());
        blockwise::gaussianSmoothMultiArray(source, dest, opt);
        return dest;
    }



    template<unsigned int DIM, class T_IN, class T_OUT>
    void defineBlockwiseFilters(){
        //typedef blockwise::BlockwiseConvolutionOptions<DIM> Opt;

        python::def("gaussianSmooth",registerConverters(&pyBlockwiseGaussianSmoothMultiArray<DIM, T_IN, T_OUT>),
            (
                python::arg("source"),
                python::arg("options"),
                python::arg("out") = python::object()
            )
        );
    }


    template<unsigned int DIM>
    void defineMultiBlocking(const std::string & clsName){

        typedef MultiBlocking<DIM> Blocking;
        typedef typename Blocking::Shape Shape;

        python::class_<Blocking>(clsName.c_str(), python::init<const Shape &, const Shape &>())
        ;
    }



    template<unsigned int DIM>
    void defineBlockwiseConvolutionOptions(const std::string & clsName){

        typedef blockwise::BlockwiseConvolutionOptions<DIM> Opt;
        python::class_<Opt>(clsName.c_str(), python::init<>())
        .add_property("stdDev", &Opt::getStdDev, &Opt::setStdDev)
        ;
    }



}
using namespace vigra;
using namespace boost::python;







BOOST_PYTHON_MODULE_INIT(blockwise)
{
    import_vigranumpy();

    python::docstring_options doc_options(true, true, false);

    defineMultiBlocking<2>("Blocking2D");
    defineMultiBlocking<3>("Blocking3D");

    defineBlockwiseConvolutionOptions<2>("BlockwiseConvolutionOptions2D");
    defineBlockwiseConvolutionOptions<3>("BlockwiseConvolutionOptions3D");

    defineBlockwiseFilters<3, float, float>();
}
