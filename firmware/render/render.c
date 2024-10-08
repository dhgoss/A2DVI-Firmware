/*
MIT License

Copyright (c) 2021 Mark Aikens
Copyright (c) 2023 David Kuder
Copyright (c) 2024 Thorsten Brehm

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdlib.h>
#include "applebus/buffers.h"
#include "config/config.h"

#include "render.h"

bool mono_rendering = false;

void DELAYED_COPY_CODE(render_init)()
{
    // clear status lines
    for (uint i=0;i<sizeof(status_line)/4;i++)
    {
        ((uint32_t*)status_line)[i] = 0xA0A0A0A0;
    }
}

void DELAYED_COPY_CODE(render_loop)()
{
    render_init();

    for(;;)
    {
#if 0
        if(romx_changed || (machinefont != current_machine))
        {
            switch_font();
            romx_changed = 0;
            machinefont = current_machine;
        }
#endif

        {
            render_debug(true);

            switch(soft_switches & SOFTSW_MODE_MASK)
            {
                case 0:
                    if(soft_switches & SOFTSW_DGR)
                    {
                        render_dgr();
                    }
                    else
                    {
                        render_lores();
                    }
                    break;
                case SOFTSW_MIX_MODE:
                    if((soft_switches & (SOFTSW_80COL | SOFTSW_DGR)) == (SOFTSW_80COL | SOFTSW_DGR))
                    {
                        render_mixed_dgr();
                    }
                    else
                    {
                        render_mixed_lores();
                    }
                    break;
                case SOFTSW_HIRES_MODE:
                    if(soft_switches & SOFTSW_DGR)
                    {
                        render_dhgr();
                    }
                    else
                    {
                        render_hires();
                    }
                    break;
                case SOFTSW_HIRES_MODE|SOFTSW_MIX_MODE:
                    if((soft_switches & (SOFTSW_80COL | SOFTSW_DGR)) == (SOFTSW_80COL | SOFTSW_DGR))
                    {
                        render_mixed_dhgr();
                    }
                    else
                    {
                        render_mixed_hires();
                    }
                    break;
                default:
                    render_text();
                    break;
            }

        }

        render_debug(false);

        update_text_flasher();

        dvi0.scanline_emulation = (internal_flags & IFLAGS_SCANLINEEMU) != 0;

        mono_rendering = (soft_switches & SOFTSW_MONOCHROME)||(internal_flags & IFLAGS_FORCED_MONO);

        frame_counter++;
    }
}
