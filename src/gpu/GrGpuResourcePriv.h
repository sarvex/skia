
/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrGpuResourcePriv_DEFINED
#define GrGpuResourcePriv_DEFINED

#include "GrGpuResource.h"

/**
 * This class allows code internal to Skia privileged access to manage the cache keys and budget
 * status of a GrGpuResource object.
 */
class GrGpuResource::ResourcePriv {
public:
    /**
     * Sets a content key for the resource. If the resource was previously cached as scratch it will
     * be converted to a content resource. Currently this may only be called once per resource. It
     * fails if there is already a resource with the same content key. TODO: make this supplant the
     * resource that currently is using the content key, allow resources' content keys to change,
     * and allow removal of a content key to convert a resource back to scratch.
     */
    bool setContentKey(const GrContentKey& contentKey) {
        return fResource->setContentKey(contentKey);
    }

    /** Removes the content key from a resource */
    void removeContentKey() { return fResource->removeContentKey(); }

    /**
     * If the resource is uncached make it cached. Has no effect on resources that are wrapped or
     * already cached.
     */
    void makeBudgeted() { fResource->makeBudgeted(); }

    /**
     * If the resource is cached make it uncached. Has no effect on resources that are wrapped or
     * already uncached. Furthermore, resources with content keys cannot be made unbudgeted.
     */
    void makeUnbudgeted() { fResource->makeUnbudgeted(); }

    /**
     * Does the resource count against the resource budget?
     */
    bool isBudgeted() const {
        bool ret = GrGpuResource::kCached_LifeCycle == fResource->fLifeCycle;
        SkASSERT(ret || !fResource->getContentKey().isValid());
        return ret;
    }

    /** 
     * If this resource can be used as a scratch resource this returns a valid scratch key.
     * Otherwise it returns a key for which isNullScratch is true. The resource may currently be
     * used as a content resource rather than scratch. Check isScratch().
     */
    const GrScratchKey& getScratchKey() const { return fResource->fScratchKey; }

    /**
     * If the resource has a scratch key, the key will be removed. Since scratch keys are installed
     * at resource creation time, this means the resource will never again be used as scratch.
     */
    void removeScratchKey() const { fResource->removeScratchKey();  }

protected:
    ResourcePriv(GrGpuResource* resource) : fResource(resource) {   }
    ResourcePriv(const ResourcePriv& that) : fResource(that.fResource) {}
    ResourcePriv& operator=(const CacheAccess&); // unimpl

    // No taking addresses of this type.
    const ResourcePriv* operator&() const;
    ResourcePriv* operator&();

    GrGpuResource* fResource;

    friend class GrGpuResource; // to construct/copy this type.
};

inline GrGpuResource::ResourcePriv GrGpuResource::resourcePriv() { return ResourcePriv(this); }

inline const GrGpuResource::ResourcePriv GrGpuResource::resourcePriv() const {
    return ResourcePriv(const_cast<GrGpuResource*>(this));
}

#endif