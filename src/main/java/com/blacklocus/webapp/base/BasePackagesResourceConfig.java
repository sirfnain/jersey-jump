package com.blacklocus.webapp.base;

import com.blacklocus.webapp.RunServer;
import com.blacklocus.webapp.app.StartupListener;
import com.blacklocus.webapp.app.scan.JerseyScannerHelper;
import com.blacklocus.webapp.app.scan.ScannerHelper;
import com.sun.jersey.api.core.InjectParam;
import com.sun.jersey.api.core.PackagesResourceConfig;
import org.apache.commons.lang3.builder.ToStringBuilder;
import org.eclipse.jetty.util.component.AbstractLifeCycle;
import org.eclipse.jetty.util.component.LifeCycle;

import javax.ws.rs.ApplicationPath;

/**
 * @author jason
 */
@ApplicationPath("/*")
public class BasePackagesResourceConfig extends PackagesResourceConfig {

    public BasePackagesResourceConfig(@InjectParam JerseyScannerHelper scannerHelper) {
        this(scannerHelper, BaseConst.BASE_PKG);
    }

    public BasePackagesResourceConfig(final ScannerHelper scannerHelper, String... basePkgs) {
        super(combine(new String[] {
                "org.codehaus.jackson.jaxrs" // json serialization
        }, basePkgs));
        // Support for initialization after context building but before serving requests.
        RunServer.SERVER.getHandler().addLifeCycleListener(new AbstractLifeCycle.AbstractLifeCycleListener() {
            @Override
            public void lifeCycleStarted(LifeCycle event) {
                for (StartupListener startupListener : scannerHelper.findImplementing(StartupListener.class)) {
                    startupListener.onStartup();
                }
            }
        });
        init();
    }

    /**
     * Last thing called by any constructor in this class.
     */
    protected void init() {
        ToStringBuilder.setDefaultStyle(BaseConst.DEFAULT_TO_STRING_STYLE);
    }

    protected static String[] combine(String pkg, String... moar) {
        return combine(new String[]{pkg}, moar);
    }

    protected static String[] combine(String[] pkgs, String... moar) {
        if (pkgs == null || moar == null) {
            return pkgs != null ? pkgs : (moar != null ? moar : null);
        }
        String[] combined = new String[pkgs.length + moar.length];
        System.arraycopy(pkgs, 0, combined, 0, pkgs.length);
        System.arraycopy(moar, 0, combined, pkgs.length, moar.length);
        return combined;
    }
}

