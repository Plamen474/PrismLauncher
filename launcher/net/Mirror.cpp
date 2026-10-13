// SPDX-License-Identifier: GPL-3.0-only
#include "Mirror.h"

#include <QLoggingCategory>

#include "net/Logging.h"

#if defined(LAUNCHER_APPLICATION)
#include "Application.h"
#include "settings/SettingsObject.h"
#endif

namespace Net {

QUrl mirrorUrl(const QUrl& original, const QString& mirrorBase)
{
    if (mirrorBase.isEmpty())
        return original;
    QString host = original.host().toLower();
    if (host.isEmpty())
        return original;
    if (!mirrorAllowlist().contains(host))
        return original;

    QString base = mirrorBase;
    // strip trailing slashes from base (default has none)
    while (base.endsWith(QLatin1Char('/')))
        base.chop(1);
    if (base.isEmpty())
        return original;

    QString path = original.path();
    // QUrl::path() always starts with '/' for hierarchical URLs; keep it.
    // Build: base + "/" + host + path
    QString rewritten = base + QLatin1Char('/') + host + path;
    QUrl out(rewritten);
    // keep query string
    if (original.hasQuery())
        out.setQuery(original.query());
    qCDebug(taskNetLogC) << "mirror rewrite:" << original.toString() << "->" << out.toString();
    return out;
}

QUrl mirrorUrl(const QUrl& original)
{
#if defined(LAUNCHER_APPLICATION)
    QString base;
    // APPLICATION may not exist in unit-test / tool contexts; fall back to default.
    // Empty setting restores stock behavior.
    try {
        if (APPLICATION && APPLICATION->settings())
            base = APPLICATION->settings()->get("MirrorBase").toString();
        else
            base = defaultMirrorBase();
    } catch (...) {
        base = defaultMirrorBase();
    }
    return mirrorUrl(original, base);
#else
    return mirrorUrl(original, defaultMirrorBase());
#endif
}

}  // namespace Net
