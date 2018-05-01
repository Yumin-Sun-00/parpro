#ifndef _X_GRADIENT_PAR
#define _X_GRADIENT_PAR

#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <omp.h>
#include "x_gradient.h"

#include <future>

using namespace std;
using namespace boost::gil;


template <typename Out> struct halfdiff_cast_channels; // forward declaration

static int range_y = 8;// amount of computations is same for any task, because all pixels need one computation

//std::promise is meant to be used only once, so I would suggest to either create this set of promises every time.


template <typename SrcView, typename DstView>
void kernel(const SrcView& src, const DstView& dst, const int& task)
{

	typedef typename channel_type<DstView>::type dst_channel_t;
    int start_y = task * range_y;
    int end_y = start_y + range_y;
    if( end_y > src.height()) end_y = src.height();

    for (int y = start_y; y < end_y; ++y)
    {
        typename SrcView::x_iterator src_it = src.row_begin(y);
        typename DstView::x_iterator dst_it = dst.row_begin(y);

        for (int x = 1; x < src.width() - 1; ++x)
        {
            static_transform(src_it[x - 1], src_it[x + 1], dst_it[x],
                                 halfdiff_cast_channels<dst_channel_t>());
        }
    }
}

template <typename SrcView, typename DstView>
void x_gradient(const SrcView& src, const DstView& dst, int num_threads) {

    static SrcView src_ = src;
    static DstView dst_ = dst;
    static int num_tasks = src.height()/range_y + 1;
	vector<future<void>> fut = vector<future<void>>(num_tasks);
    for(int i = 0; i < num_tasks; i++){
        fut[i] = async(launch::async, kernel<SrcView, DstView>, std::ref(src_), std::ref(dst_), i);
    }

}
#endif // !_X_GRADIENT_PAR_
