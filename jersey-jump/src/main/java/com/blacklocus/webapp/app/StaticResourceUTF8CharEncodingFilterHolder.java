/**
 * Copyright 2014 BlackLocus
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.blacklocus.webapp.app;

import org.apache.commons.lang.CharEncoding;
import org.eclipse.jetty.servlet.FilterHolder;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import java.io.IOException;

/**
 * Ensures that static resources are all served with UTF-8 encoding, and so assumes all such textual resources are
 * <em>actually</em> UTF-8.
 *
 * @author Jason Dunkelberger (dirkraft)
 */
public class StaticResourceUTF8CharEncodingFilterHolder extends FilterHolder {
    public StaticResourceUTF8CharEncodingFilterHolder() {
        super(new Filter() {
            @Override
            public void init(FilterConfig filterConfig) throws ServletException {
                // no-op
            }

            @Override
            public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
                response.setCharacterEncoding(CharEncoding.UTF_8);
                chain.doFilter(request, response);
            }

            @Override
            public void destroy() {
                // no-op
            }
        });
    }
}
