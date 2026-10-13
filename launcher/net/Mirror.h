// SPDX-License-Identifier: GPL-3.0-only
/*
 * Mirror rewrite for personal pull-through cache.
 * URL contract: https://mcassets.ifbw.uk/<original-host>/<original-path>?<query>
 */
#pragma once

#include <QSet>
#include <QString>
#include <QUrl>

namespace Net {

inline const QSet<QString>& mirrorAllowlist()
{
    static const QSet<QString> hosts = {
        QStringLiteral("resources.download.minecraft.net"),
        QStringLiteral("libraries.minecraft.net"),
        QStringLiteral("launchermeta.mojang.com"),
        QStringLiteral("launcher.mojang.com"),
        QStringLiteral("piston-meta.mojang.com"),
        QStringLiteral("piston-data.mojang.com"),
        QStringLiteral("meta.fabricmc.net"),
        QStringLiteral("maven.fabricmc.net"),
        QStringLiteral("meta.prismlauncher.org"),
        QStringLiteral("api.modrinth.com"),
        QStringLiteral("cdn.modrinth.com"),
    };
    return hosts;
}

inline QString defaultMirrorBase()
{
    return QStringLiteral("https://mcassets.ifbw.uk");
}

/// Pure rewrite: unchanged if mirrorBase is empty or host not allowlisted,
/// otherwise mirrorBase + "/" + host + path, keeping query string.
QUrl mirrorUrl(const QUrl& original, const QString& mirrorBase);

/// Production wrapper: reads MirrorBase setting (empty restores stock behavior).
/// Declared here, defined in Mirror.cpp (needs Application settings when available).
QUrl mirrorUrl(const QUrl& original);

}  // namespace Net
