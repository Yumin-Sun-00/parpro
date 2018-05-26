#ifndef _X_GRADIENT_PAR
#define _X_GRADIENT_PAR
#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include "x_gradient.h"
#include <thread>
#include <mutex>

using namespace boost::gil;

template <typename Out>
struct halfdiff_cast_channels; // forward declaration

template <typename SrcView, typename DstView>
void *x_gradient_kernel(const SrcView &src, const DstView &dst,
                        int &start, int &chunk_size, std::mutex &mtx)
{
    typedef typename channel_type<DstView>::type dst_channel_t;
    int local_start;

    while (true)
    {
        mtx.lock();
        local_start = start;
        if (src.height() - start < 1)
        {
            mtx.unlock();
            break;
        }

        if (src.height() - local_start < chunk_size)
            chunk_size = src.height() - local_start;

        start += chunk_size;
        mtx.unlock();

        for (int y = local_start; y < local_start + chunk_size; ++y)
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
}

template <typename SrcView, typename DstView>
void x_gradient(const SrcView &src, const DstView &dst, int num_threads)
{
    int start = 0;
    int chunk_size = 16;
    std::mutex mtx;

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i)
    {
        threads.push_back(std::thread(x_gradient_kernel<SrcView, DstView>, \
                                      std::ref(src), std::ref(dst), \
                                      std::ref(start), std::ref(chunk_size), std::ref(mtx)));
    }

    for (auto &th : threads)
        th.join();
}
#endif
