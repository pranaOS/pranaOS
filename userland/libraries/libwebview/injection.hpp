/**
 * @file injection.hpp
 * @author Krisna Pranav
 * @brief injection
 * @version 6.0
 * @date 2025-04-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace LibWebView
{
    namespace JS
    {
        std::string const onLoadHTMLInjection = R"(
            class Queue {{
                constructor() {{
                    this.elements = {{}};
                    this.head = 0;
                    this.tail = 0;
                }}

                enqueue(element) {{
                    this.elements[this.tail] = element;
                    this.tail++;
                }}

                dequeue() {{
                    const item = this.elements[this.head];
                    delete this.elements[this.head];
                    this.head++;
                    return item;
                }}

                peek() {{
                    return this.elements[this.head];
                }}

                length() {{
                    return this.tail - this.head;
                }}

                isEmpty() {{
                    return this.length == 0;
                }}
            }}

            class IndexAllocator {{
                constructor(count) {{
                    this.queue = new Queue();

                    for(let i = 0; i < count; i++) {{
                        this.queue.enqueue(i);
                    }}
                }}

                allocate() {{
                    return this.queue.dequeue();
                }}

                deallocate(element) {{
                    this.queue.enqueue(element);
                }}
            }}

            class WebView {{
                static MAX_RESULTS = 1000;

                constructor() {{
                    this.results = {{}};
                    this.allocator = new IndexAllocator(WebView.MAX_RESULTS);
                }}

                __free_result__(index) {{
                    this.allocator.deallocate(index);
                }}

                invoke(name, ...args) {{
                    const index = this.allocator.allocate();

                    let promise = new Promise((resolve, reject) => {{
                            this.results[index] = {{
                            resolve: resolve,
                            reject: reject
                        }};
                    }});

                    {0}.postMessage(
                        JSON.stringify({{
                            index: index,
                            func: name,
                            args: Array.from(args)
                        }})
                    );
                    return promise;
                }}
            }}

            let webview = new WebView();
        )";

        std::string const onResultResolveInjection = "webview.results[{0}].resolve({1}); webview.__free_result__({0});";

        std::string const onResultRejectInjection = "webview.results[{0}].reject({1}); webview.__free_result__({0});";
    } // namespace JS

} // namespace LibWebView
