/*
 * Copyright (C) 2012 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RAMSES_CAPU_GENERIC_FILESYSTEMITERATOR_H
#define RAMSES_CAPU_GENERIC_FILESYSTEMITERATOR_H

#include <ramses-capu/os/File.h>
#include <vector>

namespace ramses_capu
{
    namespace generic
    {
        template<typename STACKTYPE>
        class FileSystemIterator
        {
        public:
            FileSystemIterator();

            void setStepIntoSubdirectories(bool value);

        protected:
            ramses_capu::File mCurrentFile;

            ramses_capu::File mCurrentDirectory;

            bool mRecurseSubDirectories;

            std::vector<STACKTYPE> mDirectoryStack;
        };


        template<typename STACKTYPE>
        inline FileSystemIterator<STACKTYPE>::FileSystemIterator()
            : mCurrentFile(""), mCurrentDirectory(""), mRecurseSubDirectories(true)
        {
        }

        template<typename STACKTYPE>
        inline void FileSystemIterator<STACKTYPE>::setStepIntoSubdirectories(bool value)
        {
            mRecurseSubDirectories = value;
        }

    }
}

#endif //RAMSES_CAPU_GENERIC_FILESYSTEMITERATOR_H
